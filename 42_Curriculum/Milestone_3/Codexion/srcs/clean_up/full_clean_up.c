/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_clean_up.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 11:56:00 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/02 12:00:56 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Joins the monitor thread, the scheduler thread, then every
 *        coder thread, then destroys every mutex and frees every
 *        heap allocation owned by the program (via @c destroy_all).
 *
 * The monitor is joined first since it is responsible for signalling
 * coders to stop; joining it guarantees every coder thread has
 * already been told to stop by the time its own join runs. The
 * scheduler thread is joined next, before the coder threads, since
 * it must also observe that signal and return on its own — if it
 * relied on a coder thread having already exited, joining it here
 * (before any coder is joined) would deadlock. Join and destroy
 * failures are logged but never abort the process, so every
 * remaining thread/mutex still gets its own attempt.
 *
 * @param prog    Pointer to the program struct; its monitor thread,
 *                scheduler thread, and every coder thread are
 *                joined. All of its mutexes and heap allocations
 *                (@c coders, @c scheduler) are destroyed/freed via
 *                @c destroy_all.
 * @param dongles Array of dongles to destroy and free (via
 *                @c destroy_all).
 */
void	clean_values(t_program *prog, t_dongle *dongles)
{
	int	i;
	int	error;

	if (pthread_join((*prog).monitor_thread, NULL))
		thread_errors(4, 0);
	if (pthread_join((*prog).scheduler->scheduler_thread, NULL))
		thread_errors(6, 0);
	i = 0;
	while(i < (*prog).total_coders)
	{
		error = pthread_join((*prog).coders[i].thread, NULL);
		if (error)
			thread_errors(2, i + 1);
		i++;
	}
	destroy_all(prog, dongles);
}

/**
 * @brief Destroys every mutex owned by the program (the three
 *        shared coder mutexes, the scheduler's priority_lock and
 * 		  turn_cond, and every dongle's mutex/cond), then frees
 * 		  @p prog->coders, @p prog->scheduler, and @p dongles.
 *
 * Every destroy is attempted regardless of whether an earlier one
 * failed; failures are logged but never abort the function, so all
 * allocations are still freed and every mutex still gets its own
 * attempt.
 *
 * @param prog    Pointer to the program struct. Its @c compile_lock,
 *                @c write_lock, @c state_lock, and
 *                @c scheduler->priority_lock / turn_cond are destroyed;
 * 				  its @c coders and @c scheduler are freed.
 * @param dongles Array of dongles whose mutexes/conds are destroyed;
 *                the array itself is freed.
 */
void destroy_all(t_program *prog, t_dongle *dongles)
{
	if (pthread_mutex_destroy(&(*prog).scheduler->priority_lock))
		mutex_destroy_errors(5, 0);
	if (pthread_cond_destroy(&(*prog).scheduler->turn_cond))
		cond_erors(2);
	cleanup_after_monitor(prog, dongles);
}

/**
 * @brief Destroys every condition variable and mutex in @p dongles,
 *        then frees the array.
 *
 * If any condition variable or mutex fails to be destroyed, an
 * error message is displayed but destruction continues, giving
 * every remaining one its own attempt. Once every element has been
 * attempted, frees @p *dongles and sets it to NULL so the caller
 * can't read or free it again afterward.
 *
 * @param dongles      Pointer to the variable holding the dongles
 *                     array's address. Freed and set to NULL.
 * @param total_dongles Number of dongles in the array (each one
 *                      assumed fully initialized: both cond and
 *                      mutex).
 */
void	clean_dongles(t_dongle **dongles, int total_dongles)
{
	int	i;

	i = 0;
	while (i < total_dongles)
	{
		if (pthread_mutex_destroy(&(*dongles)[i].lock))
			mutex_destroy_errors(1, i + 1);
		i++;
	}
	free(*dongles);
	*dongles = NULL;
}