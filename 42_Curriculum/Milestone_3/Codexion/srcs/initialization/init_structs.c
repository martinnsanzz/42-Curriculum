/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 11:20:13 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 13:57:10 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Extracts the CLI rules and packs them into @p prog.
 *
 * @param argv CLI arguments.
 * @param prog Pointer to the program struct holding the program data;
 *             its @c coders array must already be allocated.
 */
int	init_program(t_program *prog, char *argv[])
{
	(*prog).compiles_required = ft_atoi(argv[6]);
	(*prog).dongle_cooldown = (size_t)ft_atoi(argv[7]);
	(*prog).start_time = get_current_time();
	(*prog).burn_out_flag = false;
	return (0);
}

int	init_scheduler(t_program *prog, char *argv[])
{
	(*prog).scheduler = ft_calloc(1, sizeof(t_schedule));
	if ((*prog).scheduler == NULL)
		return (1);
	(*prog).scheduler->burn_out = &(*prog).burn_out_flag;
	(*prog).scheduler->sched_arg = argv[8];
	(*prog).scheduler->coders = &(*prog).coders;
	return (0);
}

/**
 * @brief Initializes and allocates the coders array inside @p prog.
 *
 * For each coder it sets an id (from 1 to N), total compiles to 0,
 * state to `IDLE`, burnout flag to false, sets the rules and links its
 * left/right dongle pointers into the already-initialized @p dongles array
 * (wrapping around so the last coder's right dongle is the first
 * dongle, and the first coder's left dongle is the last).
 *
 * @param prog    Pointer to the program struct to initialize.
 *                Its @c coders field is allocated and written into.
 * @param dongles Pointer to an already-initialized array of
 *                @p prog->total_coders mutexes.
 * @param argv	  CLI arguments.
 *
 * @return 0 on success (allocation successful).
 * @return 1 on failure (allocation failed); @p prog->coders is NULL.
 */
int	init_coders(t_program *prog, t_dongle *dongles, char *argv[])
{
	int	i;

	(*prog).coders = ft_calloc((*prog).total_coders, sizeof(t_coder));
	if ((*prog).coders == NULL)
		return (1);
	i = 0;
	while (i < (*prog).total_coders)
	{
		(*prog).coders[i].id = (i + 1);
		(*prog).coders[i].total_compiles = 0;
		(*prog).coders[i].total_coders = &(prog)->total_coders;
		(*prog).coders[i].priority = false;
		(*prog).coders[i].burn_out = &(prog)->burn_out_flag;
		set_coder_rules(&(*prog).coders[i], prog, argv);
		(*prog).coders[i].start_time = &(*prog).start_time;
		(*prog).coders[i].state = IDLE;
		set_coder_dongles(&(*prog).coders[i], dongles, i);
		(*prog).coders[i].compile_lock = &(prog)->compile_lock;
		(*prog).coders[i].write_lock = &(prog)->write_lock;
		(*prog).coders[i].state_lock = &(prog)->state_lock;
		i++;
	}
	return (0);
}

/**
 * @brief Initializes and allocates the dongles array (t_dongle).
 *
 * Allocates an array of @p total_dongles structs, then initializes
 * every condition variable first, and every mutex second, each in
 * its own pass. Splitting the two passes means that if a mutex
 * fails to initialize partway through, every condition variable is
 * already known to be valid (all of them were initialized in the
 * first pass), and only the mutexes initialized so far in the
 * second pass need destroying. If a condition variable fails to
 * initialize partway through the first pass, no mutex has been
 * touched yet, so only the condition variables initialized so far
 * need destroying. Either way, the caller never receives a
 * partially-initialized array.
 *
 * @param total_dongles Number of dongles to allocate and initialize
 *                       (equal to the number of coders).
 * @param dongles        Output parameter. On success, points to a
 *                       newly allocated array of @p total_dongles
 *                       fully initialized dongles. On failure, set
 *                       to NULL.
 *
 * @return 0 on success (allocation and initialization successful).
 * @return 1 on failure; @p *dongles is NULL, no leaks or dangling
 *         mutexes/condition variables.
 */
int	init_dongles(int total_dongles, t_dongle **dongles)
{
	int	i;
	int	error;

	*dongles = ft_calloc(total_dongles, sizeof(t_dongle));
	if (*dongles == NULL)
		return (1);
	i = 0;
	while (i < total_dongles)
	{
		(*dongles)[i].last_release = 0;
		error = pthread_cond_init(&(*dongles)[i].cond, NULL);
		if (error)
			return (clean_failed_cond(dongles, i), 1);
		i++;
	}
	i = 0;
	while (i < total_dongles)
	{
		error = pthread_mutex_init(&(*dongles)[i].lock, NULL);
		if (error)
			return (clean_failed_mutex(dongles, total_dongles, i), 1);
		i++;
	}
	return (0);
}
