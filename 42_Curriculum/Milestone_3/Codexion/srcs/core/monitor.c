/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:36:24 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 12:33:31 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../codexion.h"

static int	check_total_compiles(t_program *prog);
static int	check_burnouts(t_program *prog);
static int	is_burnt_out(t_coder *coder);

/**
 * @brief Monitor thread entry point: watches every coder until the
 *        program's run is over.
 *
 * Repeatedly checks two independent stop conditions — all coders
 * finished, or a coder burnt out — and exits as soon as either is
 * true. Runs for the lifetime of the program; the coder threads and
 * @c clean_values rely on this thread returning to know the run is
 * over.
 *
 * @param pointer Cast to @c t_program*; the program struct to monitor.
 *
 * @return NULL
 */
void	*monitor(void *pointer)
{
	t_program *prog;

	prog = (t_program *)pointer;
	while (1)
	{
		if (check_total_compiles(prog) || check_burnouts(prog))
		break ;
	}
	return (NULL);
}

/**
 * @brief Checks whether every coder has reached `FINISH` state.
 *
 * @param prog Program struct holding the coders array to check.
 *
 * @return 1 if all coders are in `FINISH` state.
 * @return 0 if at least one coder is not finished yet.
 */
static int check_total_compiles(t_program *prog)
{
	int	i;
	int	finished_coders;

	i = 0;
	finished_coders = 0;
	while (i < (*prog).total_coders)
	{
		if (get_state(&(*prog).coders[i]) == FINISH)
			finished_coders += 1;
		i++;
	}
	if (finished_coders == (*prog).total_coders)
		return (1);
	return (0);
}

/**
 * @brief Checks whether a single coder has exceeded its burnout time.
 *
 * A coder currently `FINISH`ed or `COMPILING` is skipped (a compile
 * in progress resets the clock once it completes, so it cannot be
 * stale yet). For any other state, reads @c last_compile under
 * @c compile_lock — the same lock @c compile_helper writes it under —
 * so the value seen is never a stale or torn write from another thread.
 *
 * @param coder Coder to check.
 *
 * @return 1 if the elapsed time since @c last_compile exceeds
 *         @c time_to_burn_out.
 * @return 0 otherwise, or if the coder is `FINISH`/`COMPILING`.
 */
static int	is_burnt_out(t_coder *coder)
{
	size_t	last_compile;
	size_t	current_time;

	if (coder->state == FINISH || coder->state == COMPILING)
		return (0);
	pthread_mutex_lock(coder->compile_lock);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(coder->compile_lock);
	current_time = get_current_time();
	return ((int)(current_time - last_compile) > coder->time_to_burn_out);
}

/**
 * @brief Scans every coder in @p prog for burnout, stopping at the
 *        first one found.
 *
 * On detecting a burnt-out coder, locks @c write_lock before setting
 * @c burn_out_flag, updating its state to `BURNOUT`, and printing the
 * status — the same lock every coder thread holds around its own
 * flag-check-then-print, so no coder can print after the flag is set
 * based on a read that happened just before it.
 *
 * @param prog Program struct holding the coders array to check.
 *
 * @return 1 if a coder was found burnt out (flag set, message printed).
 * @return 0 if no coder is currently burnt out.
 */
static int	check_burnouts(t_program *prog)
{
	int	i;

	i = 0;
	if ((*prog).total_coders == 1)
		return (1);
	while (i < (*prog).total_coders)
	{
		if (is_burnt_out(&(*prog).coders[i]))
		{
			pthread_mutex_lock(&(*prog).write_lock);
			(*prog).burn_out_flag = true;
			set_state(&(*prog).coders[i], BURNOUT);
			display_status(&(*prog).coders[i]);
			pthread_mutex_unlock(&(*prog).write_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

