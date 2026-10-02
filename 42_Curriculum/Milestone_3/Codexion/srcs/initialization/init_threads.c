/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 14:15:59 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/02 12:00:39 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Initializes the program's three shared mutexes (compile,
 *        write, and state) and creates the monitor thread.
 *
 * Each mutex is initialized in sequence; if one fails, every mutex
 * already initialized before it is destroyed before returning, so no
 * partially-initialized lock set is left behind. If the monitor
 * thread itself fails to create, all three mutexes (now fully
 * initialized) are destroyed before returning, since the monitor
 * thread failing means the program cannot proceed. Any further
 * cleanup (dongles, coders, scheduler) is the caller's
 * responsibility.
 *
 * @param prog Pointer to the program struct; its @c compile_lock,
 *             @c write_lock, and @c state_lock are initialized, and
 *             its @c monitor_thread is created.
 *
 * @return 0 on success (all three mutexes and the monitor thread
 *         created).
 * @return 1 on failure; any mutexes already initialized are
 *         destroyed before returning.
 */
int	init_monitor_thread(t_program *prog)
{
	int	error;

	error = pthread_create(&(*prog).monitor_thread, NULL, &monitor, (void *)&(*prog));
	if (error)
	{
		thread_errors(3, 0);
		pthread_mutex_destroy(&(*prog).compile_lock);
		pthread_mutex_destroy(&(*prog).write_lock);
		pthread_mutex_destroy(&(*prog).state_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Initializes priority_lock and turn_cond, then creates the
 *        scheduler thread.
 *
 * Each pthread object is initialized in sequence; if one fails,
 * everything already initialized before it is destroyed before
 * returning, so no partially-initialized lock/cond is left behind.
 * This is the one place that owns their lifecycle — @c init_scheduler
 * never touches either.
 *
 * @param scheduler Pointer to the already-allocated scheduler
 *                  struct; its @c priority_lock and @c turn_cond are
 *                  initialized and its @c scheduler_thread created.
 *
 * @return 0 on success.
 * @return 1 on failure; anything already initialized is destroyed.
 */
int	init_scheduler_thread(t_schedule *scheduler)
{
	if (pthread_mutex_init(&(*scheduler).priority_lock, NULL))
		return (mutex_init_errors(5, 0), 1);
	if (pthread_cond_init(&(*scheduler).turn_cond, NULL))
	{
		pthread_mutex_destroy(&(*scheduler).priority_lock);
		return (cond_erors(1), 1);
	}
	if (pthread_create(&(*scheduler).scheduler_thread, NULL, &sched_routine,
			(void *)&(*scheduler)))
	{
		thread_errors(5, 0);
		pthread_mutex_destroy(&(*scheduler).priority_lock);
		pthread_cond_destroy(&(*scheduler).turn_cond);
		return (1);
	}
	return (0);
}

/**
 * @brief Creates one thread per coder, each running @c coder_routine.
 *
 * If a thread fails to create partway through, joins every thread
 * already created so far, frees @p prog->coders and sets it to NULL,
 * and returns — the caller never works with a partially-created set
 * of threads, and later cleanup (e.g. @c free((*prog).coders) again)
 * is safe since @c free(NULL) is a no-op.
 *
 * @param prog Pointer to the program struct; its @c coders array is
 *             read from (each coder's @c thread field is written
 *             into), and freed/set to NULL on failure.
 *
 * @return 0 on success (every thread created).
 * @return 1 on failure (thread creation failed at some index).
 */
int	init_coder_threads(t_program *prog)
{
	int	i;
	int	error;

	i = 0;
	while (i < (*prog).total_coders)
	{
		error = pthread_create(&(*prog).coders[i].thread, NULL, coder_routine,
				(void *)&(*prog).coders[i]);
		if (error)
		{
			thread_errors(1, i + 1);
			while (i-- > 0)
				pthread_join((*prog).coders[i].thread, NULL);
			free((*prog).coders);
			(*prog).coders = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
