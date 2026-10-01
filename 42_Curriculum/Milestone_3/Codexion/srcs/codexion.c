/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/01 15:07:56 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	program_initializer(char **argv, t_program *prog, t_dongle **dongles);

/**
 * @brief Program entry point: validates the arguments and runs the
 *        whole simulation.
 *
 * Calls check_argv() to validate argc and argv. Then stores the coder
 * count in prog.total_coders, because the init functions read it, and
 * hands over to program_initializer(), which initialises, runs and
 * cleans up everything. By the time it returns 0, the simulation has
 * finished and all resources are released.
 *
 * @param argc Argument count.
 * @param argv CLI arguments: number_of_coders, time_to_burnout,
 *             time_to_compile, time_to_debug, time_to_refactor,
 *             number_of_compiles_required, dongle_cooldown, scheduler.
 *
 * @return 0 if the simulation ran to completion.
 * @return 1 if the arguments are invalid or an initialisation stage
 *         failed.
 */
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
 * @brief Initialise, run and clean up the whole simulation.
 *
 * Stages, in this order:
 * 1. init_program(): parses the CLI rules into @p prog and sets
 *    start_time.
 * 2. init_scheduler(): must precede init_coders(), since each coder
 *    stores a pointer to @p prog->scheduler. The dongles do not
 *    depend on it.
 * 3. init_dongles(), then init_coders(). init_program() must already
 *    have set start_time, because init_coders() copies it into each
 *    coder's last_compile. The reverse order would leave last_compile
 *    uninitialised and cause an immediate false burn-out.
 * 4. Threads: monitor, scheduler, coders. They are created only after
 *    @p prog and every coder are fully populated, so no thread can
 *    observe a half-initialised structure.
 * 5. clean_values(): runs after the simulation ends and releases
 *    everything.
 *
 * If a stage fails, the resources created by the earlier stages are
 * released through the matching cleanup function (free_scheduler,
 * clean_dongles, cleanup_pre_threads, cleanup_after_monitor,
 * destroy_all).
 *
 * @param argv    CLI arguments, forwarded to the init functions.
 * @param prog    Program struct to initialise. total_coders must
 *                already be set by the caller.
 * @param dongles Output parameter: array of @p prog->total_coders
 *                dongles, allocated by init_dongles().
 *
 * @return 0 if the simulation ran and was cleaned up.
 * @return 1 if a stage failed. The stages that had already succeeded
 *         are cleaned up, except those noted below.
 */
static int	program_initializer(char **argv, t_program *prog, t_dongle **dongles)
{
	if (init_program(prog, argv))
		return (1);
	if (init_scheduler(prog, argv))
		return (1);
	if (init_dongles((*prog).total_coders, dongles))
		return (free_scheduler(&(*prog).scheduler), 1);
	if (init_coders(prog, *dongles, argv))
	{
		clean_dongles(dongles, (*prog).total_coders);
		free_scheduler(&(*prog).scheduler);
		return (1);
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
