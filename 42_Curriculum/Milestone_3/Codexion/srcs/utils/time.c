/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 09:32:06 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 11:28:09 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

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
int	get_program_time(size_t start, size_t current_time)
{
	return ((int)current_time - (int)start);
}

/**
 * @brief Delays the program @p miliseconds in chunks of 100ms.
 *
 * For every loop it checks if any coder has burnout to stop.
 *
 * @return 0 If delays hasnt being interrumpted.
 * @return 1 If delay is interrupted.
 */
int interruptible_sleep(t_coder *coder, int miliseconds)
{
	size_t start;

	start = get_current_time();
	while((int)(get_current_time() - start) < miliseconds)
	{
		if (*(*coder).burn_out == true)
			return (1);
		usleep(100);
	}
	return (0);
}