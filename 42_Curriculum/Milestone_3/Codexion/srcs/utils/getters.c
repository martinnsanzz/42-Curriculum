/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 11:14:50 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/02 12:53:16 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Safely returns the state of the coder.
 *
 * It lock the state_lock so it can't be modified when called
 * by @fn set_state().
 *
 * @return The state of the coder.
 */
t_coder_state	get_coder_state(t_coder *coder)
{
	t_coder_state	state;

	pthread_mutex_lock(coder->state_lock);
	state = coder->state;
	pthread_mutex_unlock(coder->state_lock);
	return (state);
}

/**
 * @brief Read a dongle's state and last release time in one step.
 *
 * Locks the dongle's own mutex once and copies both fields, so the
 * caller never sees a FREE state paired with an outdated
 * last_release, which two separate reads could produce if the dongle
 * were released in between. The lock is held only for the copy.
 *
 * @param dongle       Dongle to read.
 * @param last_release Output. Time of the last release, in ms since
 *                     the epoch (0 if never released).
 *
 * @return DONGLE_FREE or DONGLE_TAKEN.
 */
t_dongle_state	get_dongle_state(t_dongle *dongle, size_t *last_release)
{
	t_dongle_state	state;

	pthread_mutex_lock(&dongle->lock);
	state = dongle->state;
	*last_release = dongle->last_release;
	pthread_mutex_unlock(&dongle->lock);
	return (state);
}

/**
 * @brief Safely returns the number of compiles a coder has done
 * 		  so far.
 *
 * It locks the compile_lock so it can't be changed on function
 * call.
 *
 * @return Number of compiles the coder has done.
 */
int	get_total_compiles(t_coder *coder)
{
	int	total;

	pthread_mutex_lock(coder->compile_lock);
	total = coder->total_compiles;
	pthread_mutex_unlock(coder->compile_lock);
	return (total);
}

/**
 * @brief Gets the current time based on the Unix Epoch in milliseconds.
 *
 * @return The current time in milliseconds since the Unix Epoch.
 * @return 0 on error.
 */
size_t	get_current_time(void)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL) == -1)
		return (time_error(1), 0);
	return (t.tv_sec * 1000 + (t.tv_usec / 1000));
}

/**
 * @brief Gives the time that has passed since the program started.
 */
size_t	get_program_time(size_t start)
{
	return (get_current_time() - start);
}
