/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 14:15:59 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 13:20:44 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Initializes the program's three shared mutexes (compile,
 *        finish, burnout) and creates the monitor thread.
 *
 * Each mutex is initialized in sequence; if one fails, every mutex
 * already initialized before it is destroyed before returning, so no
 * partially-initialized lock set is left behind. If the monitor
 * thread itself fails to create, all three mutexes (now fully
 * initialized) are destroyed and @p prog->coders is freed, since the
 * monitor thread failing means the program cannot proceed.
 *
 * @param thread Output parameter; on success, holds the created
 *               monitor thread.
 * @param prog   Pointer to the program struct; its three mutex fields
 *               are initialized, and @c coders is freed on final
 *               failure.
 *
 * @return 0 on success (all three mutexes and the monitor thread
 *         created).
 * @return 1 on failure; any mutexes already initialized are
 *         destroyed.
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
 * @brief Initializes and creates all threads of the program.
 *
 * Creates one thread per coder, each running @c print_hello with a
 * pointer to its own @c t_coder as argument. If a thread fails to
 * create partway through, it joins every thread already created so
 * far, frees the coders array, and returns — the caller never works
 * with a partially-created set of threads.
 *
 * @param prog Pointer to the program struct; its @c coders array is
 *             read from (each coder's thread field is written into),
 *             and freed on failure.
 *
 * @return 0 on success (every thread created).
 * @return 1 on failure (thread creation failed at some index);
 *         @p prog->coders is freed and set to NULL.
 */
int	init_coder_threads(t_program *prog)
{
	int	i;
	int	error;

	i = 0;
	while (i < (*prog).total_coders)
	{
		(*prog).coders[i].start_time = &(*prog).start_time;
		(*prog).coders[i].last_compile = (*prog).start_time;
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
