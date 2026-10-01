/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 14:23:23 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/01 15:07:00 by masanz-s         ###   ########.fr       */
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
 * - total_coders == 1: only one dongle exists, so the coder takes its
 *   left dongle, sleeps until it burns out, sets BURNOUT, prints the
 *   status and releases the dongle.
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
		set_state(coder, FINISH);
		return (NULL);
	}
	if (*(*coder).total_coders == 1)
	{
		pthread_mutex_lock(&coder->l_dongle->lock);
		display_dongle(coder, "left");
		interruptible_sleep(coder, coder->time_to_burn_out);
		set_state(coder, BURNOUT);
		display_status(coder);
		return (pthread_mutex_unlock(&coder->l_dongle->lock), NULL);
	}
	while (!*coder->burn_out && get_state(coder) != FINISH)
	{
		if (compile(coder) || debugging(coder) || refactor(coder))
			break ;
		if (get_total_compiles(coder) == *(coder->compiles_required))
			set_state(coder, FINISH);
	}
	return (arg);
}

/**
 * @brief Compile stage: wait for a turn, take the dongles, compile.
 *
 * Calls wait_for_turn() to block until the scheduler grants access,
 * then lock_dongles() and compile_helper(). On success the coder goes
 * back to IDLE.
 *
 * @param coder Coder running the stage.
 *
 * @return 0 on success.
 * @return 1 if the turn was refused (heap full or burn-out) or
 *         compile_helper() failed.
 */
static int compile(t_coder *coder)
{
	if (wait_for_turn(coder))
		return (1);
	lock_dongles(coder);
	if (compile_helper(coder))
		return (1);
	set_state(coder, IDLE);
	return (0);
}

/**
 * @brief Perform the compile while holding both dongles.
 *
 * Under write_lock, checks burn_out and, if it is not set, sets the
 * state to COMPILING and prints the status. If burn_out was already
 * set, releases both dongles and returns 1 without compiling.
 * Otherwise, under compile_lock, records last_compile and increments
 * total_compiles, then sleeps for time_to_compile via
 * interruptible_sleep(). The dongles are released after the sleep.
 *
 * @param coder Coder holding both dongles.
 *
 * @return 0 on success.
 * @return 1 if burn_out was set before the compile began.
 * @return The non-zero value of interruptible_sleep() if the sleep
 *         was interrupted. The dongles are released in that case too.
 */
static int	compile_helper(t_coder *coder)
{
	int	error;
	int	burnt;

	pthread_mutex_lock((*coder).write_lock);
	burnt = *(*coder).burn_out;
	if (!burnt)
	{
		set_state(coder, COMPILING);
		display_status(coder);
	}
	pthread_mutex_unlock((*coder).write_lock);
	if (burnt)
	{
		pthread_mutex_unlock(&coder->l_dongle->lock);
		return (pthread_mutex_unlock(&coder->r_dongle->lock), 1);
	}
	pthread_mutex_lock((*coder).compile_lock);
	(*coder).last_compile = get_current_time();
	(*coder).total_compiles += 1;
	pthread_mutex_unlock((*coder).compile_lock);
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
static int debugging(t_coder *coder)
{
	int	burnt;

	pthread_mutex_lock((*coder).write_lock);
	burnt = *(*coder).burn_out;
	if (!burnt)
	{
		set_state(coder, DEBUGGING);
		display_status(coder);
	}
	pthread_mutex_unlock((*coder).write_lock);
	if (burnt)
		return (1);
	if (interruptible_sleep(coder, coder->time_to_debug))
		return (1);
	set_state(coder, IDLE);
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
static int refactor(t_coder *coder)
{
	int	burnt;

	pthread_mutex_lock((*coder).write_lock);
	burnt = *(*coder).burn_out;
	if (!burnt)
	{
		set_state(coder, REFACTORING);
		display_status(coder);
	}
	pthread_mutex_unlock((*coder).write_lock);
	if (burnt)
		return (1);
	if (interruptible_sleep(coder, coder->time_to_refactor))
		return (1);
	set_state(coder, IDLE);
	return (0);
}
