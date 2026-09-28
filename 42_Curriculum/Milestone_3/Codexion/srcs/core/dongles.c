/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:37:13 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 14:59:54 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void wait_for_dongle(t_dongle *dongle, size_t cooldown);

/**
 * @brief Locks a coder's left and right dongles in a fixed order.
 *
 * Even-numbered coders lock their left dongle before their right;
 * odd-numbered coders lock right before left. This consistent,
 * opposite ordering between neighbors is what prevents circular
 * wait: two coders sharing a dongle can never both be holding one
 * mutex while blocked on the other's.
 *
 * @param coder Pointer to the coder acquiring its dongles. Blocks
 *              until both @c l_dongle->lock and @c r_dongle->lock
 *              are held.
 */
void	lock_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		wait_for_dongle(coder->l_dongle, (size_t)*coder->dongle_cooldown);
		display_dongle(coder, "left");
		wait_for_dongle(coder->r_dongle, (size_t)*coder->dongle_cooldown);
		display_dongle(coder, "right");
	}
	else
	{
		wait_for_dongle(coder->r_dongle, (size_t)*coder->dongle_cooldown);
		display_dongle(coder, "right");
		wait_for_dongle(coder->l_dongle, (size_t)*coder->dongle_cooldown);
		display_dongle(coder, "left");
	}
}

/**
 * @brief Locks a dongle and blocks until its cooldown has elapsed.
 *
 * Locks @p dongle->lock, then checks whether the dongle is still cooling
 * down. If it is, computes the absolute deadline at which the
 * cooldown expires and sleeps on @p dongle->cond via
 * pthread_cond_timedwait, which atomically releases the lock while
 * waiting and reacquires it on wake. The check is re-evaluated in a
 * while loop (not if) to guard against spurious wakeups and against
 * @c last_release changing between iterations. Returns with
 * @p dongle->lock held.
 *
 * @param dongle   Pointer to the dongle to acquire.
 * @param cooldown Cooldown duration in milliseconds.
 */
static void wait_for_dongle(t_dongle *dongle, size_t cooldown)
{
	struct timespec	deadline;
	size_t			wake_at;

	pthread_mutex_lock(&dongle->lock);
	while(is_cooldown(dongle->last_release, cooldown))
	{
		wake_at = dongle->last_release + cooldown;
		deadline.tv_sec = wake_at / 1000;
		deadline.tv_nsec = (wake_at % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->cond, &dongle->lock, &deadline);
	}
}

/**
 * @brief Releases a coder's dongles, recording the release time.
 *
 * For each dongle, while still holding its lock, writes the current
 * time into @c last_release and broadcasts on its condition
 * variable to wake any coder waiting on that dongle, then unlocks
 * it. Writing the timestamp and broadcasting before unlocking
 * guarantees the next thread to acquire the lock always observes an
 * up-to-date @c last_release, with no window where a stale value
 * could be read. Dongles are released in the same left/right order
 * used by @c lock_dongles, mirrored by coder parity.
 *
 * @param coder Pointer to the coder releasing its dongles.
 */
void	unlock_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		coder->l_dongle->last_release = get_current_time();
		pthread_cond_broadcast(&coder->l_dongle->cond);
		pthread_mutex_unlock(&coder->l_dongle->lock);
		coder->r_dongle->last_release = get_current_time();
		pthread_cond_broadcast(&coder->r_dongle->cond);
		pthread_mutex_unlock(&coder->r_dongle->lock);
	}
	else
	{
		coder->r_dongle->last_release = get_current_time();
		pthread_cond_broadcast(&coder->r_dongle->cond);
		pthread_mutex_unlock(&coder->r_dongle->lock);
		coder->l_dongle->last_release = get_current_time();
		pthread_cond_broadcast(&coder->l_dongle->cond);
		pthread_mutex_unlock(&coder->l_dongle->lock);
	}
}