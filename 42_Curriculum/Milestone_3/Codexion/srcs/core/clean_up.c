/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 11:56:00 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 13:30:24 by masanz-s         ###   ########.fr       */
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
 *                       destroyed via @c destroy_all.
 * @param dongles        Array of dongle mutexes to destroy and free
 *                       (via @c destroy_all).
 */
void	clean_values(pthread_t monitor_thread, t_program *prog, t_dongle *dongles)
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

	destroy_all(prog, dongles);

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
void destroy_all(t_program *prog, t_dongle *dongles)
{
	int	i;
	int error;

	if (pthread_mutex_destroy(&(*prog).compile_lock))
		mutex_destroy_errors(2, 0);
	if (pthread_mutex_destroy(&(*prog).write_lock))
		mutex_destroy_errors(3, 0);
	if (pthread_mutex_destroy(&(*prog).state_lock))
		mutex_destroy_errors(4, 0);

	i = 0;
	while (i < (*prog).total_coders)
	{
		error = pthread_cond_destroy(&dongles[i].cond);
		if (error)
			cond_erors(2, i + 1);
		error = pthread_mutex_destroy(&dongles[i].lock);
		if (error)
			mutex_destroy_errors(1, i + 1);
		i++;
	}
	free(dongles);
}

/**
 * @brief Cleans up after a failed pthread_cond_init in init_dongles.
 *
 * Called when the condition variable at index @p i failed to
 * initialize. Destroys every condition variable at indices below
 * @p i (the ones that did succeed, in the first pass), then frees
 * the dongles array itself. No mutex has been initialized yet at
 * this point (mutex init only starts once the whole condition
 * variable pass has completed), so none are destroyed here.
 *
 * @param dongles Pointer to the dongles array. Freed and set to
 *                NULL by this function.
 * @param i       Index at which pthread_cond_init failed; also the
 *                count of condition variables successfully
 *                initialized before it.
 */
void	clean_failed_cond(t_dongle **dongles, int i)
{
	cond_erors(1, i + 1);
	while (i-- > 0)
		pthread_cond_destroy(&(*dongles)[i].cond);
	free(*dongles);
	*dongles = NULL;
}

/**
 * @brief Cleans up after a failed pthread_mutex_init in init_dongles.
 *
 * Called when the mutex at index @p i failed to initialize. All
 * @p total_dongles condition variables were already successfully
 * initialized in the prior pass, so every one of them is destroyed.
 * Only the mutexes at indices below @p i succeeded, so only those
 * are destroyed. Finally frees the dongles array itself.
 *
 * @param dongles       Pointer to the dongles array. Freed and set
 *                      to NULL by this function.
 * @param total_dongles Total number of dongles allocated (and thus
 *                      the number of condition variables to destroy).
 * @param i             Index at which pthread_mutex_init failed;
 *                      also the count of mutexes successfully
 *                      initialized before it.
 */
void	clean_failed_mutex(t_dongle **dongles, int total_dongles, int i)
{
	mutex_init_errors(1, i + 1);
	while (i-- > 0)
		pthread_mutex_destroy(&(*dongles)[i].lock);
	while (total_dongles-- > 0)
		pthread_cond_destroy(&(*dongles)[total_dongles].cond);
	free(*dongles);
	*dongles = NULL;
}