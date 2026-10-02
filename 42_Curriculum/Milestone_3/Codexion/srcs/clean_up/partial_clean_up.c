/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_clean_up.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 13:59:07 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/02 12:13:43 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

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
void	clean_failed_mutex(t_dongle **dongles, int i)
{
	mutex_init_errors(1, i + 1);
	while (i-- > 0)
		pthread_mutex_destroy(&(*dongles)[i].lock);
	free(*dongles);
	*dongles = NULL;
}

/**
 * @brief Cleans up everything allocated before any thread exists.
 *
 * Called when a stage that runs before @c init_monitor_thread fails
 * (e.g. @c init_scheduler). At this point every dongle is fully
 * initialized (both cond and mutex), so @c clean_dongles destroys
 * and frees the whole array. @p prog->coders is freed directly (it
 * owns no mutexes of its own — only pointers into @p prog and the
 * dongles). @p (*prog).scheduler->heap is freed before @p prog->scheduler
 * is freed directly: it was either never allocated (NULL, safe to free)
 * or allocated but never given an initialized mutex yet, so there is
 * nothing inside it to destroy.
 *
 * @param prog    Pointer to the program struct; its @c coders and
 *                @c scheduler are freed.
 * @param dongles Array of fully-initialized dongles to destroy and
 *                free.
 */
void	cleanup_pre_threads(t_program *prog, t_dongle *dongles)
{
	clean_dongles(&dongles, (*prog).total_coders);
	free((*prog).coders);
	free_scheduler(&(*prog).scheduler);
}

/**
 * @brief Cleans up everything allocated once the monitor thread's
 *        mutexes exist, but before the scheduler thread's does.
 *
 * Called when @c init_scheduler_thread fails. By this point
 * @c init_monitor_thread has already succeeded, so @c compile_lock,
 * @c write_lock, and @c state_lock were all successfully
 * initialized and are destroyed here. @c priority_lock is not
 * touched: @c init_scheduler_thread already destroyed it itself on
 * failure (its own @c pthread_create failed after its own
 * @c pthread_mutex_init succeeded), so destroying it again here
 * would be a double destroy. The rest of the cleanup (dongles,
 * @c coders, @c scheduler) is delegated to @c cleanup_pre_threads.
 *
 * @param prog    Pointer to the program struct; its three shared
 *                mutexes are destroyed, then @c coders and
 *                @c scheduler are freed via @c cleanup_pre_threads.
 * @param dongles Array of fully-initialized dongles to destroy and
 *                free.
 */
void	cleanup_after_monitor(t_program *prog, t_dongle *dongles)
{
	if (pthread_mutex_destroy(&(*prog).compile_lock))
		mutex_destroy_errors(2, 0);
	if (pthread_mutex_destroy(&(*prog).write_lock))
		mutex_destroy_errors(3, 0);
	if (pthread_mutex_destroy(&(*prog).state_lock))
		mutex_destroy_errors(4, 0);
	cleanup_pre_threads(prog, dongles);
}

/**
 * @brief Frees a scheduler's heap array and the struct itself.
 *
 * Called whenever @p scheduler was allocated (by @c init_scheduler)
 * but priority_lock/turn_cond were never initialized (so nothing
 * pthread-related needs destroying). Safe to call on a NULL
 * @p *scheduler.
 *
 * @param scheduler Pointer to the scheduler pointer to free and set
 *                  to NULL.
 */
void	free_scheduler(t_schedule **scheduler)
{
	if (*scheduler == NULL)
		return ;
	free((*scheduler)->heap);
	free(*scheduler);
	*scheduler = NULL;
}