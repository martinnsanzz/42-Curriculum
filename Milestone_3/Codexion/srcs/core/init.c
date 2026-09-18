/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:54 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/18 09:43:39 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"
#include "sys/resource.h"

static int	init_dongles(int total_dongles, pthread_mutex_t **dongles);
static int	init_coders(t_program *prog, pthread_mutex_t *dongles);
static int	init_monitor_thread(pthread_t *thread, t_program *prog);
static int	init_coder_threads(t_program *prog);

/**
 * @brief Initializes all core program data structures in sequence.
 *
 * Allocates and sets up dongles, then coders (in that order, since
 * each coder needs its left/right dongle mutexes to already exist).
 * Frees any previously allocated resource if a later stage fails,
 * so the caller never receives a partially-initialized @p prog or
 * @p dongles on error. Also gets start_time of program fo reference
 * later on.
 *
 * @param argv    CLI arguments.
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
int	program_initializer(char **argv, t_program *prog, pthread_mutex_t **dongles)
{
	pthread_t	monitor_thread;

	if (init_dongles((*prog).total_coders, dongles))
		return (1);
	if (init_coders(prog, *dongles))
	{
		free(*dongles);
		*dongles = NULL;
		return (1);
	}
	get_rules(argv, prog);
	if (init_monitor_thread(&monitor_thread, prog))
		return (pthread_mutex_destroy_all(prog, (*dongles)), 1);
	(*prog).start_time = get_current_time();
	if (init_coder_threads(prog))
		return (pthread_mutex_destroy_all(prog, (*dongles)), 1);
	clean_values(monitor_thread, prog, *dongles);
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
	int	i;

	(*prog).coders = ft_calloc((*prog).total_coders, sizeof(t_coder));
	if ((*prog).coders == NULL)
		return (1);
	i = 0;
	while (i < (*prog).total_coders)
	{
		(*prog).coders[i].id = (i + 1);
		(*prog).coders[i].total_compiles = 0;
		(*prog).coders[i].state = INIT;
		(*prog).coders[i].burn_out = &(prog)->burn_out_flag;
		(*prog).coders[i].compile_lock = &(prog)->compile_lock;
		(*prog).coders[i].burnout_lock = &(prog)->burnout_lock;
		(*prog).coders[i].finish_lock = &(prog)->finish_lock;
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
			mutex_init_errors(1, i + 1);
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

/**
 * @brief Initializes the program's three shared mutexes (compile,
 *        finish, burnout) and creates the monitor thread.
 *
 * Each mutex is initialized in sequence; if one fails, every mutex
 * already initialized before it is destroyed before returning, so no
 * partially-initialized lock set is left behind. If the monitor
 * thread itself fails to create, all three mutexes (now fully
 * initialized) are destroyed and @p prog->coders is freed, since the
 * monitor thread failing means the program cannot proceed.
 *
 * @param thread Output parameter; on success, holds the created
 *               monitor thread.
 * @param prog   Pointer to the program struct; its three mutex fields
 *               are initialized, and @c coders is freed on final
 *               failure.
 *
 * @return 0 on success (all three mutexes and the monitor thread
 *         created).
 * @return 1 on failure; any mutexes already initialized are
 *         destroyed.
 */
static int	init_monitor_thread(pthread_t *thread, t_program *prog)
{
	int	error;

	if (pthread_mutex_init(&(*prog).compile_lock, NULL))
		return (mutex_init_errors(2, 0), 1);
	if (pthread_mutex_init(&(*prog).finish_lock, NULL))
		return (pthread_mutex_destroy(&(*prog).compile_lock),
			mutex_init_errors(3, 0), 1);
	if (pthread_mutex_init(&(*prog).burnout_lock, NULL))
	{
		pthread_mutex_destroy(&(*prog).compile_lock);
		pthread_mutex_destroy(&(*prog).finish_lock);
		mutex_init_errors(4, 0);
		return (1);
	}
	error = pthread_create(thread, NULL, &monitor, (void *)&(*prog));
	if (error)
	{
		thread_errors(3, 0);
		pthread_mutex_destroy(&(*prog).compile_lock);
		pthread_mutex_destroy(&(*prog).finish_lock);
		pthread_mutex_destroy(&(*prog).burnout_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Initializes and creates all threads of the program.
 *
 * Creates one thread per coder, each running @c print_hello with a
 * pointer to its own @c t_coder as argument. If a thread fails to
 * create partway through, it joins every thread already created so
 * far, frees the coders array, and returns — the caller never works
 * with a partially-created set of threads.
 *
 * @param prog Pointer to the program struct; its @c coders array is
 *             read from (each coder's thread field is written into),
 *             and freed on failure.
 *
 * @return 0 on success (every thread created).
 * @return 1 on failure (thread creation failed at some index);
 *         @p prog->coders is freed and set to NULL.
 */
static int	init_coder_threads(t_program *prog)
{
	int	i;
	int	error;

	i = 0;
	while (i < (*prog).total_coders)
	{
		(*prog).coders[i].start_time = &(*prog).start_time;
		error = pthread_create(&(*prog).coders[i].thread, NULL, print_hello,
				(void *)&(*prog).coders[i]);
		if (error)
		{
			thread_errors(1, i + 1);
			while (i-- > 0)
				pthread_join((*prog).coders[i].thread, NULL);
			free((*prog).coders);
			(*prog).coders = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
