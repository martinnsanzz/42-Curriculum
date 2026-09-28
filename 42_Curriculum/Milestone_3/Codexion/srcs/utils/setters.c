/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 11:15:17 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 14:57:37 by masanz-s         ###   ########.fr       */
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
 * @brief Set the left and right dongles of each coder in @p prog
 */
void	set_coder_dongles(t_program *prog, t_dongle *dongles)
{
	int i;

	i = 0;
	while (i < (*prog).total_coders)
	{
		(*prog).coders[i].r_dongle = &dongles[i];
		if (i == 0)
			(*prog).coders[i].l_dongle = &dongles[(*prog).total_coders - 1];
		else
			(*prog).coders[i].l_dongle = &dongles[i - 1];
		i++;
	}
}

/**
 * @brief Set the rules of each coder in @p prog from the CLI
 */
void	set_coder_rules(char *argv[], t_program *prog)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		(*prog).coders[i].time_to_burn_out = (size_t)ft_atoi(argv[2]);
		(*prog).coders[i].time_to_compile = (size_t)ft_atoi(argv[3]);
		(*prog).coders[i].time_to_debug = (size_t)ft_atoi(argv[4]);
		(*prog).coders[i].time_to_refactor = (size_t)ft_atoi(argv[5]);
		(*prog).coders[i].compiles_required = &(*prog).compiles_required;
		(*prog).coders[i].dongle_cooldown = &(*prog).dongle_cooldown;
		(*prog).coders[i].last_compile = (*prog).start_time;
		i++;
	}
}