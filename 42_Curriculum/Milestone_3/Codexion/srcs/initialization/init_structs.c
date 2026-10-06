/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 11:20:13 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:21:57 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Extracts the CLI rules that belong to the whole program
 *        (not per-coder) and packs them into @p prog.
 *
 * Sets @c compiles_required, @c dongle_cooldown, and @c start_time
 * (captured here, before any thread exists, so no thread can ever
 * observe it uninitialized). It also initializes the program mutexes
 * (compile_lock, write_lock, state_lock). Each mutex is initialized
 * in sequence; if one fails, every mutex already initialized before
 * it is destroyed before returning, so no partially-initialized lock
 * set is left behindIf anPer-coder rules are set separately by
 * @c set_coder_rules inside @c init_coders.
 *
 * @param prog Pointer to the program struct to fill in.
 * @param argv CLI arguments.
 */
int	init_program(t_program *prog, char *argv[])
{
	(*prog).compiles_required = ft_atoi(argv[6]);
	(*prog).start_time = get_current_time();
	(*prog).burn_out_flag = false;
	(*prog).all_finish = false;
	if (pthread_mutex_init(&(*prog).compile_lock, NULL))
		return (mutex_init_errors(2, 0), 1);
	if (pthread_mutex_init(&(*prog).write_lock, NULL))
	{
		mutex_init_errors(3, 0);
		return (pthread_mutex_destroy(&(*prog).compile_lock), 1);
	}
	if (pthread_mutex_init(&(*prog).state_lock, NULL))
	{
		pthread_mutex_destroy(&(*prog).compile_lock);
		pthread_mutex_destroy(&(*prog).write_lock);
		mutex_init_errors(4, 0);
		return (1);
	}
	return (0);
}

/**
 * @brief Allocates and initializes @p prog->scheduler.
 *
 * Links @p scheduler->burn_out to @p prog->burn_out_flag and
 * @p scheduler->coders to the address of @p prog->coders, so the
 * scheduler always observes the current values of both, and copies
 * the scheduler argument (argv[8]) in. Must be called after
 * @c init_coders, since @c sched_routine (via @p scheduler->coders)
 * expects @p prog->coders to already be a valid, populated array.
 * Does not initialize @c priority_lock or create the scheduler
 * thread — that happens later, in @c init_scheduler_thread.
 * It lastly initializes the mutex @c priority lock.
 *
 * @param prog Pointer to the program struct; its @c scheduler field
 *             is allocated and written into.
 * @param argv CLI arguments.
 *
 * @return 0 on success (allocation and mutex init succesfull).
 * @return 1 on failure (allocation or mutex failed); @p prog->scheduler
 * 			is NULL.
 */
int	init_scheduler(t_program *prog, char *argv[])
{
	(*prog).scheduler = ft_calloc(1, sizeof(t_schedule));
	if ((*prog).scheduler == NULL)
		return (1);
	(*prog).scheduler->heap = ft_calloc((*prog).total_coders,
			sizeof(t_request));
	if ((*prog).scheduler->heap == NULL)
	{
		free((*prog).scheduler);
		(*prog).scheduler = NULL;
		return (1);
	}
	(*prog).scheduler->burn_out = &(*prog).burn_out_flag;
	(*prog).scheduler->all_finish = &(*prog).all_finish;
	(*prog).scheduler->sched_arg = argv[8];
	return (0);
}

/**
 * @brief Initializes and allocates the dongles array (t_dongle).
 *
 * Allocates an array of @p total_dongles structs, then initializes
 * and every mutex. The caller never receives a partially-initialized
 * array.
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
		(*dongles)[i].state = DONGLE_FREE;
		(*dongles)[i].last_release = 0;
		error = pthread_mutex_init(&(*dongles)[i].lock, NULL);
		if (error)
			return (clean_failed_mutex(dongles, i), 1);
		i++;
	}
	return (0);
}

/**
 * @brief Initializes and allocates the coders array inside @p prog.
 *
 * For each coder it sets an id (from 1 to N), total compiles to 0,
 * state to `IDLE`, burnout flag to false, sets the rules, links its
 * left/right dongle pointers into the already-initialized @p dongles
 * array (wrapping around so the last coder's right dongle is the
 * first dongle, and the first coder's left dongle is the last), and
 * links @c schedule to @p prog->scheduler so the coder can enqueue
 * itself onto the scheduler's heap. Must be called after
 * @c init_scheduler, since @p prog->scheduler must already exist.
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
		(*prog).coders[i].last_compile = (*prog).start_time;
		(*prog).coders[i].start_time = &(*prog).start_time;
		(*prog).coders[i].state = IDLE;
		set_coder_dongles(&(*prog).coders[i], dongles, i);
		(*prog).coders[i].compile_lock = &(prog)->compile_lock;
		(*prog).coders[i].write_lock = &(prog)->write_lock;
		(*prog).coders[i].state_lock = &(prog)->state_lock;
		(*prog).coders[i].schedule = (*prog).scheduler;
		i++;
	}
	return (0);
}
