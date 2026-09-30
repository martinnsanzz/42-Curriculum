/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 11:15:17 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 12:17:33 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Safely sets the state of @p coder.
 */
void	set_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(coder->state_lock);
	coder->state = state;
	pthread_mutex_unlock(coder->state_lock);
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
 * @brief Set the rules of each coder in @p prog from the @p argv
 */
void	set_coder_rules(t_coder *coder, t_program *prog, char *argv[])
{
		(*coder).time_to_burn_out = (size_t)ft_atoi(argv[2]);
		(*coder).time_to_compile = (size_t)ft_atoi(argv[3]);
		(*coder).time_to_debug = (size_t)ft_atoi(argv[4]);
		(*coder).time_to_refactor = (size_t)ft_atoi(argv[5]);
		(*coder).compiles_required = &(*prog).compiles_required;
		(*coder).dongle_cooldown = &(*prog).dongle_cooldown;
		(*coder).last_compile = (*prog).start_time;
}