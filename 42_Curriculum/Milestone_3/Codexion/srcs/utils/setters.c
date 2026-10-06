/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 11:15:17 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:25:35 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Safely sets the state of @p coder.
 */
void	set_coder_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(coder->state_lock);
	coder->state = state;
	pthread_mutex_unlock(coder->state_lock);
}

/**
 * @brief Safaly sets the state of @p dongle.
 *
 * If the @p state is `DONGLE_FREE` it sets the @c release_time
 * of the @p dongle to the current time in ms.
 */
void	set_dongle_state(t_dongle *dongle, t_dongle_state state)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->state = state;
	if (state == DONGLE_FREE)
		dongle->last_release = get_current_time();
	pthread_mutex_unlock(&dongle->lock);
}

/**
 * @brief Set the left and right dongles of each coder based on @p index
 */
void	set_coder_dongles(t_coder *coder, t_dongle *dongles, int index)
{
	(*coder).r_dongle = &dongles[index];
	if (index == 0)
		(*coder).l_dongle = &dongles[*(*coder).total_coders - 1];
	else
		(*coder).l_dongle = &dongles[index - 1];
}

/**
 * @brief Set the rules of each coder from the @p argv and @p prog
 */
void	set_coder_rules(t_coder *coder, t_program *prog, char *argv[])
{
	(*coder).time_to_burn_out = (size_t)ft_atoi(argv[2]);
	(*coder).time_to_compile = (size_t)ft_atoi(argv[3]);
	(*coder).time_to_debug = (size_t)ft_atoi(argv[4]);
	(*coder).time_to_refactor = (size_t)ft_atoi(argv[5]);
	(*coder).dongle_cooldown = (size_t)ft_atoi(argv[7]);
	(*coder).compiles_required = &(*prog).compiles_required;
}
