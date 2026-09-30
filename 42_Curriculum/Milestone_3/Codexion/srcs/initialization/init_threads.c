/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 14:15:59 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 17:18:02 by 2002mssm02       ###   ########.fr       */
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
	if (pthread_mutex_init(&(*prog).compile_lock, NULL))
		return (mutex_init_errors(2, 0), 1);
	if (pthread_mutex_init(&(*prog).write_lock, NULL))
		return (pthread_mutex_destroy(&(*prog).compile_lock),
			mutex_init_errors(3, 0), 1);
	if (pthread_mutex_init(&(*prog).state_lock, NULL))
	{
		pthread_mutex_destroy(&(*prog).compile_lock);
		pthread_mutex_destroy(&(*prog).write_lock);
		mutex_init_errors(4, 0);
		return (1);
	}
	if (pthread_create(&(*prog).monitor_thread, NULL, &monitor, (void *)&(*prog)))
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
 * @brief Initializes the scheduler's priority_lock and creates the
 *        scheduler thread.
 *
 * If the mutex initializes but @c pthread_create fails, the mutex is
 * destroyed here before returning, so the caller never has to guess
 * whether it needs destroying elsewhere.
 *
 * @param scheduler Pointer to the already-allocated scheduler
 *                  struct (from @c init_scheduler); its
 *                  @c priority_lock is initialized and its
 *                  @c scheduler_thread is created.
 *
 * @return 0 on success (mutex initialized and thread created).
 * @return 1 on failure; @c priority_lock is destroyed if it was
 *         initialized before the failure.
 */
int	init_scheduler_thread(t_schedule *scheduler)
{
	if (pthread_mutex_init(&(*scheduler).priority_lock, NULL))
		return ((mutex_init_errors(5, 0), 1));
	if (pthread_create(&(*scheduler).scheduler_thread, NULL, &sched_routine, (void *)&(*scheduler)))
	{
		thread_errors(5, 0);
		pthread_mutex_destroy(&(*scheduler).priority_lock);
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
