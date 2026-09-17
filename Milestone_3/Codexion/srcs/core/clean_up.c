/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 11:56:00 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 17:01:12 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Joins the monitor thread, then every coder thread, then
 *        destroys all mutexes (program-level locks and dongles) and
 *        frees the coders array.
 *
 * The monitor is joined first since it is responsible for signalling
 * coders to stop; joining it guarantees every coder thread is either
 * finished or has been told to stop by the time its own join runs.
 * Join and destroy failures are logged but never abort the process,
 * so every remaining thread/mutex still gets its own attempt.
 *
 * @param monitor_thread The monitor thread to join first.
 * @param prog           Pointer to the program struct; its @c coders
 *                       array is joined thread-by-thread and freed at
 *                       the end. Its three program-level mutexes are
 *                       destroyed via @c pthread_mutex_destroy_all.
 * @param dongles        Array of dongle mutexes to destroy and free
 *                       (via @c pthread_mutex_destroy_all).
 */
void	clean_values(pthread_t monitor_thread, t_program *prog, pthread_mutex_t *dongles)
{
	int	i;
	int	error;

	if (pthread_join(monitor_thread, NULL))
		thread_errors(4, 0);
	i = 0;
	while(i < (*prog).total_coders)
	{
		error = pthread_join((*prog).coders[i].thread, NULL);
		if (error)
			thread_errors(2, i + 1);
		i++;
	}
	i = 0;

	pthread_mutex_destroy_all(prog, dongles);

    free((*prog).coders);
}

/**
 * @brief Destroys the program's three shared mutexes (compile, finish,
 *        burnout) and every dongle mutex, then frees @p dongles.
 *
 * Every destroy is attempted regardless of whether an earlier one
 * failed; failures are logged but never abort the function, so
 * @p dongles is always freed and every mutex gets its own attempt.
 *
 * @param prog    Pointer to the program struct holding the three
 *                shared mutexes to destroy.
 * @param dongles Array of dongle mutexes to destroy and free.
 */
void pthread_mutex_destroy_all(t_program *prog, pthread_mutex_t *dongles)
{
	int	i;
	int error;

	if (pthread_mutex_destroy(&(*prog).compile_lock))
		mutex_destroy_errors(2, 0);
	if (pthread_mutex_destroy(&(*prog).finish_lock))
		mutex_destroy_errors(3, 0);
	if (pthread_mutex_destroy(&(*prog).burnout_lock))
		mutex_destroy_errors(4, 0);

	i = 0;
	while(i < (*prog).total_coders)
	{
		error = pthread_mutex_destroy(&dongles[i]);
		if (error)
			mutex_destroy_errors(1, i + 1);
		i++;
	}
	free(dongles);
}
