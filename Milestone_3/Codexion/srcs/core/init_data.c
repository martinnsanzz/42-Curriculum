/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:54 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 14:20:01 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"
#include "sys/resource.h"

static int	init_dongles(int total_dongles, pthread_mutex_t **dongles);
static int	init_coders(t_program *prog, pthread_mutex_t *dongles);

/**
 * @brief Initializes all core program data structures in sequence.
 *
 * Allocates and sets up dongles, then coders (in that order, since
 * each coder needs its left/right dongle mutexes to already exist).
 * Frees any previously allocated resource if a later stage fails,
 * so the caller never receives a partially-initialized @p prog or
 * @p dongles on error.
 *
 * @param prog    Pointer to the program struct to initialize.
 *                Its @c coders and @c total_coders fields are read
 *                from and written into by this function.
 * @param dongles Output parameter. On success, points to a newly
 *                allocated array of @p prog->total_coders initialized
 *                mutexes. On failure, set to NULL.
 *
 * @return 0 on success (all fields fully initialized).
 * @return 1 on failure; @p prog and @p dongles are left in a clean
 *         state (no leaks or midway allocations).
 */
int	data_initializer(t_program *prog, pthread_mutex_t **dongles)
{
	if (init_dongles((*prog).total_coders, dongles))
		return (1);
	if (init_coders(prog, *dongles))
	{
		free(*dongles);
		*dongles = NULL;
		return (1);
	}
	return (0);
}

/**
 * @brief Initializes and allocates the coders array inside @p prog.
 *
 * For each coder it sets an id (from 1 to N), total compiles to 0,
 * state to `INIT`, burnout flag to false, and links its left/right
 * dongle pointers into the already-initialized @p dongles array
 * (wrapping around so the last coder's right dongle is the first
 * dongle, and the first coder's left dongle is the last).
 *
 * @param prog    Pointer to the program struct to initialize.
 *                Its @c coders field is allocated and written into.
 * @param dongles Pointer to an already-initialized array of
 *                @p prog->total_coders mutexes.
 *
 * @return 0 on success (allocation successful).
 * @return 1 on failure (allocation failed); @p prog->coders is NULL.
 */
static int	init_coders(t_program *prog, pthread_mutex_t *dongles)
{
	t_coder_state	state;
	int				i;

	(*prog).coders = ft_calloc((*prog).total_coders, sizeof(t_coder));
	if ((*prog).coders == NULL)
		return (1);
	state = INIT;
	i = 0;
	while (i < (*prog).total_coders)
	{
		(*prog).coders[i].id = (i + 1);
		(*prog).coders[i].total_compiles = 0;
		(*prog).coders[i].coder_state = state;
		(*prog).coders[i].is_burn_out = false;
		(*prog).coders[i].r_dongle = &dongles[i];
		if (i == 0)
			(*prog).coders[i].l_dongle = &dongles[(*prog).total_coders - 1];
		else
			(*prog).coders[i].l_dongle = &dongles[i - 1];
		i++;
	}
	return (0);
}
/**
 * @brief Initializes and allocates the dongles array (mutexes).
 *
 * Allocates an array of @p total_dongles mutexes and initializes
 * each one. If a mutex fails to initialize partway through, it
 * destroys and frees every dongle already initialized so far,
 * so the caller never receives a partially-initialized array.
 *
 * @param total_dongles Number of dongle mutexes to allocate and
 *                       initialize (equal to the number of coders).
 * @param dongles        Output parameter. On success, points to a
 *                       newly allocated array of initialized mutexes.
 *                       On failure, set to NULL.
 *
 * @return 0 on success (allocation and mutex initialization successful).
 * @return 1 on failure; @p *dongles is NULL, no leaks or dangling mutexes.
 */
static int	init_dongles(int total_dongles, pthread_mutex_t **dongles)
{
	int	i;
	int	error;

	*dongles = ft_calloc(total_dongles, sizeof(pthread_mutex_t));
	if (*dongles == NULL)
		return (1);
	i = 0;
	while (i < total_dongles)
	{
		error = pthread_mutex_init(&(*dongles)[i], NULL);
		if (error)
		{
			mutex_errors(1, i + 1);
			while (i-- > 0)
				pthread_mutex_destroy(&(*dongles)[i]);
			free(*dongles);
			*dongles = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
