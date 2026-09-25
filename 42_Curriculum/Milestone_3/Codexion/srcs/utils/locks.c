/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 15:03:26 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 13:57:06 by masanz-s         ###   ########.fr       */
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
 * @brief Safely returns the state of the coder.
 *
 * It lock the state_lock so it can't be modified when called
 * by @fn set_state().
 *
 * @return The state of the coder.
 */
t_coder_state	get_state(t_coder *coder)
{
	t_coder_state	state;

	pthread_mutex_lock(coder->state_lock);
	state = coder->state;
	pthread_mutex_unlock(coder->state_lock);
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
