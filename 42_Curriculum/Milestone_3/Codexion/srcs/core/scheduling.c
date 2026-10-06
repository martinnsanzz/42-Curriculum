/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduling.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 13:22:37 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/06 09:51:43 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	wait_until(t_schedule *s, size_t wake_at);
static void	grant_or_wait(t_schedule *s);

/**
 * @brief Scheduler thread entry point: grants dongles in key order.
 *
 * Holds priority_lock for the whole run, releasing it only while
 * sleeping. Loops grant_or_wait() until burn_out or all_finish is set,
 * then broadcasts once more so no coder stays asleep in wait_for_turn.
 *
 * @param pointer Cast to @c t_schedule*; the scheduler to run.
 *
 * @return NULL
 */
void	*sched_routine(void *pointer)
{
	t_schedule	*s;

	s = (t_schedule *)pointer;
	pthread_mutex_lock(&s->priority_lock);
	while (!(*s->burn_out || *s->all_finish))
		grant_or_wait(s);
	pthread_cond_broadcast(&s->turn_cond);
	pthread_mutex_unlock(&s->priority_lock);
	return (NULL);
}

/**
 * @brief Compute the heap key for a coder's request.
 *
 * FIFO: returns an increasing arrival sequence (s->next_seq++), so the
 * oldest request has the smallest key.
 * Any other policy (EDF): returns the coder's deadline,
 * last_compile + time_to_burn_out, so the most urgent coder has the
 * smallest key.
 * Caller must hold priority_lock, since next_seq is shared.
 *
 * @param s     Scheduler holding the policy and the sequence counter.
 * @param coder Coder whose request is being keyed.
 *
 * @return The key used to order the request in the min-heap.
 */
size_t	compute_key(t_schedule *s, t_coder *coder)
{
	if (ft_strcmp(s->sched_arg, FIFO) == 0)
		return (s->next_seq);
	else
		return (coder->last_compile + coder->time_to_burn_out);
}

/**
 * @brief Block a coder until the scheduler grants it a turn.
 *
 * Under priority_lock: clears the coder's priority flag, pushes it
 * into the heap and broadcasts turn_cond to wake the scheduler. Then
 * sleeps on turn_cond until priority is set or burn_out is raised. The
 * wait is a loop, so spurious wakeups are harmless. When priority is
 * set, the scheduler has already reserved both dongles for this coder.
 * The lock is released exactly once on every exit path.
 *
 * @param coder Coder requesting its turn.
 *
 * @return 0 if the turn was granted.
 * @return 1 if the heap was full or the run ended by burn-out.
 */
int	wait_for_turn(t_coder *coder)
{
	t_schedule	*s;
	int			stop;

	s = coder->schedule;
	pthread_mutex_lock(&s->priority_lock);
	coder->priority = false;
	if (heap_push(s, coder) == -1)
	{
		pthread_mutex_unlock(&s->priority_lock);
		return (1);
	}
	pthread_cond_broadcast(&s->turn_cond);
	while (!coder->priority && !*coder->burn_out)
		pthread_cond_wait(&s->turn_cond, &s->priority_lock);
	stop = *coder->burn_out;
	pthread_mutex_unlock(&s->priority_lock);
	return (stop);
}

/**
 * @brief Sleep on turn_cond until an absolute time or a wakeup.
 *
 * Used when the root coder's dongles are free but still cooling down.
 * Caller must hold priority_lock. Returns early if turn_cond is
 * broadcast, and the caller's loop re-evaluates everything.
 *
 * @param s       Scheduler owning turn_cond and priority_lock.
 * @param wake_at Absolute time in ms since the epoch.
 */
static void	wait_until(t_schedule *s, size_t wake_at)
{
	struct timespec	deadline;

	deadline.tv_sec = wake_at / 1000;
	deadline.tv_nsec = (wake_at % 1000) * 1000000;
	pthread_cond_timedwait(&s->turn_cond, &s->priority_lock, &deadline);
}

static void	grant_or_wait(t_schedule *s)
{
	size_t	wake_at;
	int		index;
	t_coder	*next;

	if (s->heap_size == 0)
		pthread_cond_wait(&s->turn_cond, &s->priority_lock);
	else
	{
		index = find_grantable(s, &wake_at);
		if (index >= 0)
		{
			next = s->heap[index].coder;
			reserve_dongles(next);
			heap_remove_at(s, (size_t)index);
			next->priority = true;
			pthread_cond_broadcast(&s->turn_cond);
		}
		else if (wake_at == 0)
			pthread_cond_wait(&s->turn_cond, &s->priority_lock);
		else
			wait_until(s, wake_at);
	}
}
