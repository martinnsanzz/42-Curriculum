/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 17:12:45 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	program_initializer(char **argv, t_program *prog, t_dongle **dongles);

int     main(int argc, char *argv[])
{
    t_program	prog;
	t_dongle	*dongles;

	if (check_argv(argc, argv) == 1)
        return (1);
	prog.total_coders = ft_atoi(argv[1]);
	if (program_initializer(argv, &prog, &dongles))
		return (1);
	return (0);
}

/**
 * @brief Initializes all core program data structures in sequence.
 *
 * Parses the CLI rules into @p prog, the allocates and sets up dongles,
 * (in that order, since @c init_program sets @p prog->start_time before
 * @c init_coder copies it into each coder's @c last_compile —
 * reversing that order would leave @c last_compile holding garbage
 * and trigger an immediate false burnout). Only once @p prog and
 * every coder are fully populated are the monitor thread and coder
 * threads created, so no thread can ever observe a partially
 * initialized @p prog or coder. Frees any previously allocated
 * resource if a later stage fails, so the caller never receives a
 * partially-initialized @p prog or @p dongles on error.
 *
 * @param argv    CLI arguments.
 * @param prog    Pointer to the program struct to initialize.
 *                Its @c coders, @c total_coders, and rule fields are
 *                read from and written into by this function.
 * @param dongles Output parameter. On success, points to a newly
 *                allocated array of @p prog->total_coders initialized
 *                dongles. On failure, set to NULL.
 *
 * @return 0 on success (all fields fully initialized, all threads
 *         running).
 * @return 1 on failure; @p prog and @p dongles are left in a clean
 *         state (no leaks, no dangling mutexes/condition variables,
 *         no threads left running).
 */
static int	program_initializer(char **argv, t_program *prog, t_dongle **dongles)
{
	init_program(prog, argv);
	if (init_dongles((*prog).total_coders, dongles))
		return (1);
	if (init_coders(prog, *dongles, argv))
		return (clean_dongles(dongles, (*prog).total_coders), 1);
	if (init_scheduler(prog, argv))
	{
		clean_dongles(dongles, (*prog).total_coders);
		return (free((*prog).coders), 1);
	}
	if (init_monitor_thread(prog))
		return (cleanup_pre_threads(prog, *dongles), 1);
	if (init_scheduler_thread((*prog).scheduler))
		return (cleanup_after_monitor(prog, *dongles), 1);
	if (init_coder_threads(prog))
		return (destroy_all(prog, *dongles), 1);
	clean_values(prog, *dongles);
	return (0);
}
