/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 14:23:23 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:26:36 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	compile(t_coder *coder);
static int	debugging(t_coder *coder);
static int	refactor(t_coder *coder);
static int	compile_helper(t_coder *coder);

/**
 * @brief Coder thread entry point: runs the compile / debug / refactor
 *        cycle until the coder finishes or the run ends.
 *
 * Special cases first:
 * - compiles_required == 0: the coder is set to FINISH and returns.
 * - total_coders == 1: only one dongle exists, so the coder marks its
 *   left dongle TAKEN, sleeps until it burns out, sets BURNOUT, prints
 *   the status and marks the dongle FREE again. The scheduler is not
 *   involved in this case.
 *
 * Otherwise it loops while burn_out is unset and the state is not
 * FINISH, running compile, debugging and refactor in order. Any stage
 * returning non-zero ends the loop. After each full cycle, the coder
 * is set to FINISH once total_compiles reaches compiles_required.
 *
 * @param arg Cast to @c t_coder*; the coder owned by this thread.
 *
 * @return NULL in the early-exit cases, @p arg otherwise.
 */
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (*(coder->compiles_required) == 0)
	{
		set_coder_state(coder, FINISH);
		return (NULL);
	}
	if (*(coder->total_coders) == 1)
	{
		set_dongle_state(coder->l_dongle, DONGLE_TAKEN);
		display_dongle(coder, "left");
		interruptible_sleep(coder, coder->time_to_burn_out);
		set_coder_state(coder, BURNOUT);
		display_status(coder);
		return (set_dongle_state(coder->l_dongle, DONGLE_FREE), NULL);
	}
	while (!*coder->burn_out && get_coder_state(coder) != FINISH)
	{
		if (compile(coder) || debugging(coder) || refactor(coder))
			break ;
		if (get_total_compiles(coder) == *(coder->compiles_required))
			set_coder_state(coder, FINISH);
	}
	return (arg);
}

/**
 * @brief Compile stage: wait for a turn, announce the dongles, compile.
 *
 * Calls wait_for_turn() to block until the scheduler grants access.
 * When it returns 0, the scheduler has already marked both dongles
 * TAKEN for this coder. Then take_dongles() prints the status and
 * compile_helper() runs the compile. On success the coder goes back
 * to IDLE.
 *
 * @param coder Coder running the stage.
 *
 * @return 0 on success.
 * @return 1 if the turn was refused (heap full or burn-out) or
 *         compile_helper() failed.
 */
static int	compile(t_coder *coder)
{
	if (wait_for_turn(coder))
		return (1);
	take_dongles(coder);
	if (compile_helper(coder))
		return (1);
	set_coder_state(coder, IDLE);
	return (0);
}

/**
 * @brief Perform the compile while owning both dongles.
 *
 * Under write_lock, checks burn_out and, if it is not set, sets the
 * state to COMPILING and prints the status. If burn_out was already
 * set, releases both dongles with unlock_dongles() and returns 1
 * without compiling. Otherwise, under compile_lock, records
 * last_compile and increments total_compiles, then sleeps for
 * time_to_compile via interruptible_sleep(). The dongles are released
 * with unlock_dongles() after the sleep, also when the sleep failed.
 *
 * @param coder Coder owning both dongles.
 *
 * @return 0 on success.
 * @return 1 if burn_out was set before the compile began.
 * @return The non-zero value of interruptible_sleep() if the sleep
 *         was interrupted.
 */
static int	compile_helper(t_coder *coder)
{
	int	error;
	int	burnt;

	pthread_mutex_lock(coder->write_lock);
	burnt = *coder->burn_out;
	if (!burnt)
	{
		set_coder_state(coder, COMPILING);
		display_status(coder);
	}
	pthread_mutex_unlock(coder->write_lock);
	if (burnt)
		return (unlock_dongles(coder), 1);
	pthread_mutex_lock(coder->compile_lock);
	coder->last_compile = get_current_time();
	coder->total_compiles += 1;
	pthread_mutex_unlock(coder->compile_lock);
	error = interruptible_sleep(coder, coder->time_to_compile);
	unlock_dongles(coder);
	return (error);
}

/**
 * @brief Debugging stage: sleep for time_to_debug.
 *
 * Under write_lock, checks burn_out and, if it is not set, sets the
 * state to DEBUGGING and prints the status. Then sleeps for
 * time_to_debug via interruptible_sleep(). On success the coder goes
 * back to IDLE.
 *
 * @param coder Coder running the stage.
 *
 * @return 0 on success.
 * @return 1 if burn_out was set or the sleep was interrupted.
 */
static int	debugging(t_coder *coder)
{
	int	burnt;

	pthread_mutex_lock(coder->write_lock);
	burnt = *coder->burn_out;
	if (!burnt)
	{
		set_coder_state(coder, DEBUGGING);
		display_status(coder);
	}
	pthread_mutex_unlock(coder->write_lock);
	if (burnt)
		return (1);
	if (interruptible_sleep(coder, coder->time_to_debug))
		return (1);
	set_coder_state(coder, IDLE);
	return (0);
}

/**
 * @brief Refactoring stage: sleep for time_to_refactor.
 *
 * Under write_lock, checks burn_out and, if it is not set, sets the
 * state to REFACTORING and prints the status. Then sleeps for
 * time_to_refactor via interruptible_sleep(). On success the coder
 * goes back to IDLE.
 *
 * @param coder Coder running the stage.
 *
 * @return 0 on success.
 * @return 1 if burn_out was set or the sleep was interrupted.
 */
static int	refactor(t_coder *coder)
{
	int	burnt;

	pthread_mutex_lock(coder->write_lock);
	burnt = *coder->burn_out;
	if (!burnt)
	{
		set_coder_state(coder, REFACTORING);
		display_status(coder);
	}
	pthread_mutex_unlock(coder->write_lock);
	if (burnt)
		return (1);
	if (interruptible_sleep(coder, coder->time_to_refactor))
		return (1);
	set_coder_state(coder, IDLE);
	return (0);
}
