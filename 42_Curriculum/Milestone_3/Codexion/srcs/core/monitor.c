/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:36:24 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:26:32 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	raise_flag(t_program *prog, bool *flag);
static int	check_total_compiles(t_program *prog);
static int	check_burnouts(t_program *prog);
static bool	is_burnt_out(t_coder *coder);

/**
 * @brief Monitor thread entry point: watches every coder until the
 *        program's run is over.
 *
 * Repeatedly checks two independent stop conditions (all coders
 * finished, or a coder burnt out) and exits as soon as either is
 * true. Sleeps 1 ms between rounds so it does not spin on a CPU core.
 * The coder threads and @c clean_values rely on this thread returning
 * to know the run is over.
 *
 * @param pointer Cast to @c t_program*; the program struct to monitor.
 *
 * @return NULL
 */
void	*monitor(void *pointer)
{
	t_program	*prog;

	prog = (t_program *)pointer;
	while (1)
	{
		if (check_total_compiles(prog) || check_burnouts(prog))
			break ;
		usleep(1000);
	}
	return (NULL);
}

/**
 * @brief Set a stop flag and wake the scheduler and every waiting coder.
 *
 * Writes the flag under priority_lock, the lock the scheduler and
 * wait_for_turn() read it under, then broadcasts turn_cond before
 * releasing it. A thread that checked the flag but has not slept yet
 * cannot miss the wakeup. Callers may hold write_lock; the order is
 * always write_lock, then priority_lock.
 *
 * @param prog Program struct owning the scheduler.
 * @param flag Flag to raise (all_finish or burn_out_flag).
 */
static void	raise_flag(t_program *prog, bool *flag)
{
	pthread_mutex_lock(&prog->scheduler->priority_lock);
	*flag = true;
	pthread_cond_broadcast(&prog->scheduler->turn_cond);
	pthread_mutex_unlock(&prog->scheduler->priority_lock);
}

/**
 * @brief Checks whether every coder has reached `FINISH` state.
 *
 * When all coders are finished, raises @c all_finish through
 * raise_flag() and prints the success message.
 *
 * @param prog Program struct holding the coders array to check.
 *
 * @return 1 if all coders are in `FINISH` state.
 * @return 0 if at least one coder is not finished yet.
 */
static int	check_total_compiles(t_program *prog)
{
	int	i;
	int	finished_coders;

	i = 0;
	finished_coders = 0;
	while (i < prog->total_coders)
	{
		if (get_coder_state(&prog->coders[i]) == FINISH)
			finished_coders += 1;
		i++;
	}
	if (finished_coders == prog->total_coders)
	{
		raise_flag(prog, &prog->all_finish);
		printf(LOG_SUCCESS, YELLOW, RESET);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks whether a single coder has exceeded its burnout time.
 *
 * Reads the coder state once through get_coder_state() and compares
 * the copy. A coder that is `FINISH`ed or `COMPILING` is skipped (a
 * compile in progress resets the clock once it completes, so it
 * cannot be stale yet). For any other state, reads @c last_compile
 * under @c compile_lock, the same lock compile_helper() writes it
 * under.
 *
 * @param coder Coder to check.
 *
 * @return 1 if the elapsed time since @c last_compile exceeds
 *         @c time_to_burn_out.
 * @return 0 otherwise, or if the coder is `FINISH`/`COMPILING`.
 */
static bool	is_burnt_out(t_coder *coder)
{
	t_coder_state	state;
	size_t			last_compile;
	size_t			current_time;

	state = get_coder_state(coder);
	if (state == FINISH || state == COMPILING)
		return (false);
	pthread_mutex_lock(coder->compile_lock);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(coder->compile_lock);
	current_time = get_current_time();
	return (current_time - last_compile > coder->time_to_burn_out);
}

/**
 * @brief Scans every coder in @p prog for burnout, stopping at the
 *        first one found.
 *
 * With a single coder, the coder thread sleeps until it burns out and
 * prints the status itself. The monitor only waits for that coder to
 * reach `BURNOUT`, then raises @c burn_out_flag so the scheduler
 * thread can exit.
 *
 * With several coders, on detecting a burnt-out coder, it locks
 * @c write_lock, raises @c burn_out_flag, sets the state to `BURNOUT`
 * and prints the status. This is the same lock every coder thread
 * holds around its own flag-check-then-print, so no coder can print
 * after the flag is set based on an earlier read. raise_flag() also
 * wakes the scheduler and every coder waiting in wait_for_turn().
 *
 * @param prog Program struct holding the coders array to check.
 *
 * @return 1 if a burnout was handled (flag set, scheduler woken).
 * @return 0 if no coder is currently burnt out.
 */
static int	check_burnouts(t_program *prog)
{
	int	i;

	if (prog->total_coders == 1)
	{
		if (get_coder_state(&prog->coders[0]) != BURNOUT)
			return (0);
		return (raise_flag(prog, &prog->burn_out_flag), 1);
	}
	i = 0;
	while (i < prog->total_coders)
	{
		if (is_burnt_out(&prog->coders[i]))
		{
			pthread_mutex_lock(&prog->write_lock);
			raise_flag(prog, &prog->burn_out_flag);
			set_coder_state(&prog->coders[i], BURNOUT);
			display_status(&prog->coders[i]);
			pthread_mutex_unlock(&prog->write_lock);
			return (1);
		}
		i++;
	}
	return (0);
}
