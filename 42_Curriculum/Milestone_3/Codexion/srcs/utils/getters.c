/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 11:14:50 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 14:55:27 by masanz-s         ###   ########.fr       */
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

/**
 * @brief Extracts the CLI rules and packs them into @p prog.
 *
 * @param argv CLI arguments.
 * @param prog Pointer to the program struct holding the program data;
 *             its @c coders array must already be allocated.
 */
void    get_rules(char *argv[], t_program *prog)
{
    (*prog).compiles_required = ft_atoi(argv[6]);
	(*prog).dongle_cooldown = (size_t)ft_atoi(argv[7]);
	(*prog).scheduler = argv[8];
	(*prog).start_time = get_current_time();
	(*prog).burn_out_flag = false;
}