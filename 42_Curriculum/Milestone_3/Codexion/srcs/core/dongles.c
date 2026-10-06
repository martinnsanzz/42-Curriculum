/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:37:13 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/06 09:51:36 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Announce that a granted coder has taken both dongles.
 */
void	take_dongles(t_coder *coder)
{
	display_dongle(coder, "left");
	display_dongle(coder, "right");
}

/**
 * @brief Release both of a coder's dongles and wake the scheduler.
 *
 * Sets the left and then the right dongle to DONGLE_FREE through
 * set_dongle_state(), which also records last_release under each
 * dongle's own lock. Then calls wake_scheduler(), so a scheduler that
 * is waiting for a release re-checks the root coder. No dongle lock
 * is held when wake_scheduler() takes priority_lock.
 *
 * @param coder Coder releasing its dongles.
 */
void	unlock_dongles(t_coder *coder)
{
	set_dongle_state(coder->l_dongle, DONGLE_FREE);
	set_dongle_state(coder->r_dongle, DONGLE_FREE);
	wake_scheduler(coder->schedule);
}

/**
 * @brief Mark both of a coder's dongles as taken.
 *
 * Sets the left and then the right dongle to DONGLE_TAKEN through
 * set_dongle_state(). Only the scheduler calls it, after can_grant()
 * returned true. Coders only ever set dongles back to free, so
 * nothing can invalidate the check between the two calls.
 * Caller must hold priority_lock.
 *
 * @param coder Coder whose dongles are reserved.
 */
void	reserve_dongles(t_coder *coder)
{
	set_dongle_state(coder->l_dongle, DONGLE_TAKEN);
	set_dongle_state(coder->r_dongle, DONGLE_TAKEN);
}

/**
 * @brief Check whether a coder can take both dongles right now.
 *
 * Reads each dongle with get_dongle_state(). It changes nothing.
 * - Either dongle TAKEN: returns false and sets @p wake_at to 0, which
 *   means "wait for a release", since no time can be predicted.
 * - Both FREE but still cooling down: returns false and sets
 *   @p wake_at to the later of the two cooldown end times, as an
 *   absolute time in ms.
 * - Both FREE and out of cooldown: returns true.
 *
 * Caller must hold priority_lock.
 *
 * @param coder   Coder at the root of the heap.
 * @param wake_at Output. 0 to wait for a release, otherwise the
 *                absolute time at which to check again.
 *
 * @return true if the coder can take both dongles.
 * @return false otherwise.
 */
bool	can_grant(t_coder *coder, size_t *wake_at)
{
	size_t	l_release;
	size_t	r_release;
	size_t	l_end;
	size_t	r_end;

	*wake_at = 0;
	if (get_dongle_state(coder->l_dongle, &l_release) == DONGLE_TAKEN)
		return (0);
	if (get_dongle_state(coder->r_dongle, &r_release) == DONGLE_TAKEN)
		return (0);
	l_end = l_release + coder->dongle_cooldown;
	r_end = r_release + coder->dongle_cooldown;
	if (r_end > l_end)
		l_end = r_end;
	if (l_end > get_current_time())
	{
		*wake_at = l_end;
		return (0);
	}
	return (1);
}

/**
 * @brief Wake the scheduler and every coder sleeping on turn_cond.
 *
 * Called by the monitor after setting burn_out_flag or all_finish, and
 * by unlock_dongles after a release. Takes priority_lock around the
 * broadcast so a thread that has just checked its predicate but not
 * yet slept cannot miss the wakeup.
 *
 * @param s Scheduler whose turn_cond is broadcast.
 */
void	wake_scheduler(t_schedule *s)
{
	pthread_mutex_lock(&s->priority_lock);
	pthread_cond_broadcast(&s->turn_cond);
	pthread_mutex_unlock(&s->priority_lock);
}
