/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 09:32:06 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:26:06 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Checks is a dongle is in cooldown or not.
 *
 * @param last_release	Time where the dongle was last used.
 * @param cooldown	Time in (ms) for a dongle to be available to
 * 					use again after being released.
 */
bool	is_cooldown(size_t last_release, size_t cooldown)
{
	return (get_current_time() - last_release < cooldown);
}

/**
 * @brief Delays the program @p miliseconds in chunks of 100ms.
 *
 * For every loop it checks if any coder has burnout to stop.
 *
 * @return 0 If delays hasnt being interrumpted.
 * @return 1 If delay is interrupted.
 */
int	interruptible_sleep(t_coder *coder, int miliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((int)(get_current_time() - start) < miliseconds)
	{
		if (*(*coder).burn_out == true)
			return (1);
		usleep(100);
	}
	return (0);
}
