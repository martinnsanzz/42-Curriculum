/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:54 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 15:08:34 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"
#include "sys/resource.h"

static int  init_coders(t_program *prog);
static int  init_dongles(t_program *prog);

/**
 * @brief Initializes all core program data structures in sequence.
 *
 * Allocates and sets up coders, dongles, and threads (in that order),
 * it frees any previously allocated resource if a later stage fails,
 * so the caller never receives a partially-initialized @p program
 * on error.
 *
 * @param prog Pointer to the program struct to initialize.
 * 			   Its fields (coders, dongles, threads, args, etc.)
 * 			   are allocated and written into by this function.
 *
 * @return 0 on success (All fields fully initialized).
 * @return 1 on failure; @p program is left in a clean state (no leaks
 * 			or midway allocations).
 *
 */
int	data_initializer(t_program *prog)
{
    int i;

	if (init_coders(prog))
		return (1);

	if (init_dongles(prog))
		return (free ((*prog).coders), 1);

	if (init_threads(prog))
	{
        i = 0;
		while(i < (*prog).rules[0])
			pthread_mutex_destroy(&(*prog).dongles[i++].lock);
		free((*prog).coders);
		free((*prog).dongles);
		return (1);
	}
	return (0);
}

/**
 * @brief Initialize and allocate coders struct inside @p prog
 *
 * For each coder it provides an id (From 1 to N), sets the total
 * compiles to 0, the state of the coder to `INIT` and the index
 * of each right and left dongle based on its `ID`.
 *
 * @param prog Pointer to the program struct to initialize.
 *
 * @return 0 on success (Allocation succesfull).
 * @return 1 on failure.
 */
static int  init_coders(t_program *prog)
{
    t_coder_state	state;
	int				i;

	state = INIT;
	(*prog).coders = ft_calloc((*prog).rules[0], sizeof(t_coder));
	if ((*prog).coders == NULL)
		return (1);

	i = 0;
	while(i < (*prog).rules[0])
	{
		(*prog).coders[i].coder_id = (i + 1);
		(*prog).coders[i].total_compiles = 0;
		(*prog).coders[i].coder_state = state;

		(*prog).coders[i].right_dongle_i = (i + 1);

		if (i == 0)
			(*prog).coders[i].left_dongle_i = (*prog).rules[0];
		else
			(*prog).coders[i].left_dongle_i = i;
		i++;
	}
	return (0);
}

/**
 * @brief Initialize and allocates dongles struct inside @p prog.
 *
 * For each dongle it sets the id of the dongle (From 1 to N), sets
 * the state of the dongle to `AVAILABLE` and initializes the mutex
 * inside the struct. On failure of mutex initialization it destroys
 * and frees previously initialized dongles so the caller never
 * receives a partially-initialized @p program dongles.
 *
 * @param prog Pointer to the program struct to initialize.
 *
 * @return 0 on success (Allocation and mutex initialization succesfull).
 * @return 1 on failure; @p prog.dongles are left clean.
 */
static int  init_dongles(t_program *prog)
{
    int	i;
	int	error;

	(*prog).dongles = ft_calloc((*prog).rules[0], sizeof(t_dongle));
	if ((*prog).dongles == NULL)
		return (1);
	i = 0;
	while(i < (*prog).rules[0])
	{
		(*prog).dongles[i].dongle_id = (i + 1);
		(*prog).dongles[i].dongle_state = AVAILABLE;
		error = pthread_mutex_init(&(*prog).dongles[i].lock, NULL);
		if (error)
		{
			mutex_errors(1, i + 1);

			while (i-- > 0)
				pthread_mutex_destroy(&(*prog).dongles[i].lock);
			free((*prog).dongles);
			(*prog).dongles = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
