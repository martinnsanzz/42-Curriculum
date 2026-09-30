/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_clean_up.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 13:59:07 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 14:49:12 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

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

void	cleanup_pre_threads(t_program *prog, t_dongle *dongles)
{
	clean_dongles(&dongles, (*prog).total_coders);
	free((*prog).coders);
	free((*prog).scheduler);
}

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