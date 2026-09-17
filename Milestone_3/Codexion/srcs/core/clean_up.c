/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 11:56:00 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 14:26:11 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Joins all coder threads, destroys all dongle mutexes, and
 *        frees both arrays.
 *
 * Every thread is joined before any mutex is destroyed, guaranteeing
 * no coder still holds a lock when its mutex is destroyed. Join or
 * destroy failures are logged but never abort the loop, so every
 * remaining thread/mutex still gets its own attempt and nothing leaks
 * partway through.
 *
 * @param total_coders Number of coders/threads/dongles to clean up.
 * @param coders       Array of coders whose threads are joined; freed
 *                      at the end.
 * @param dongles      Array of mutexes to destroy; freed at the end.
 *
 * @note Only call this when @p coders and @p dongles are both valid,
 *       fully-created arrays (e.g. after a successful @c init_threads).
 *       For a failure path where threads were never created, use
 *       @c pthread_mutex_destroy_all on @p dongles alone instead.
 */
void	clean_values(int total_coders, t_coder *coders, pthread_mutex_t *dongles)
{
	int	i;
	int	error;

	i = 0;
	while(i < total_coders)
	{
		error = pthread_join(coders[i].thread, NULL);
		if (error)
			thread_errors(2, i + 1);
		i++;
	}
	i = 0;
	while(i < total_coders)
	{
		error = pthread_mutex_destroy(&dongles[i]);
		if (error)
			mutex_errors(2, i + 1);
		i++;
	}
    free(coders);
	free(dongles);
}

/**
 * @brief Destroys every mutex in @p dongles and frees the array.
 *
 * Used on failure paths where the dongles array was fully initialized
 * but no threads exist yet to join (e.g. @c init_threads failed), so
 * only the dongles need tearing down.
 *
 * @param dongles       Array of mutexes to destroy; freed at the end.
 * @param total_dongles Number of mutexes in @p dongles.
 */
void pthread_mutex_destroy_all(pthread_mutex_t *dongles, int total_dongles)
{
	int	i;

	i = 0;
	while(i < total_dongles)
		pthread_mutex_destroy(&dongles[i++]);
	free(dongles);
}
