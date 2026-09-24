/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 15:03:26 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/24 16:24:40 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	set_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(coder->state_lock);
	coder->state = state;
	pthread_mutex_unlock(coder->state_lock);
}

t_coder_state	get_state(t_coder *coder)
{
	t_coder_state	state;

	pthread_mutex_lock(coder->state_lock);
	state = coder->state;
	pthread_mutex_unlock(coder->state_lock);
	return (state);
}

int	get_total_compiles(t_coder *coder)
{
	int	total;

	pthread_mutex_lock(coder->compile_lock);
	total = coder->total_compiles;
	pthread_mutex_unlock(coder->compile_lock);
	return (total);
}