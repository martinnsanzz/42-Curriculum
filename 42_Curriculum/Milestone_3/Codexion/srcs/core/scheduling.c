/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduling.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 13:22:37 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/01 15:05:26 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Scheduler thread entry point: grants dongle access in key order.
 *
 * Holds priority_lock for the whole run, releasing it only while
 * sleeping in pthread_cond_wait. While the heap is empty it sleeps on
 * turn_cond. Otherwise it pops the coder with the smallest key, sets
 * its priority flag and broadcasts turn_cond so the waiting coders
 * re-check their predicate. Stops when burn_out or all_finish is set,
 * and broadcasts once more so no coder stays asleep in wait_for_turn.
 *
 * @param pointer Cast to @c t_schedule*; the scheduler to run.
 *
 * @return NULL
 */
void	*sched_routine(void *pointer)
{
	t_schedule	*s;
	t_coder		*next;

	s = (t_schedule *)pointer;
	pthread_mutex_lock(&s->priority_lock);
	while(!(*(s->burn_out) || *(s->all_finish)))
	{
		if (s->heap_size == 0)
		{
			pthread_cond_wait(&s->turn_cond, &s->priority_lock);
			continue ;
		}
		next = heap_pop(s);
		next->priority = true;
		pthread_cond_broadcast(&s->turn_cond);
	}
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
		return (s->next_seq++);
	else
		return (coder->last_compile + coder->time_to_burn_out);
}

/**
 * @brief Block a coder until the scheduler grants it a turn.
 *
 * Under priority_lock: clears the coder's priority flag, pushes it
 * into the heap and broadcasts turn_cond to wake the scheduler. Then
 * sleeps on turn_cond until priority is set or burn_out is raised. The
 * wait is a loop, so spurious wakeups are harmless. The lock is
 * released exactly once on every exit path.
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
 * @brief Wake the scheduler and every coder sleeping on turn_cond.
 *
 * Called by the monitor after setting burn_out_flag or all_finish.
 * Takes priority_lock around the broadcast so a thread that has just
 * checked the flag but not yet slept cannot miss the wakeup.
 *
 * @param s Scheduler whose turn_cond is broadcast.
 */
void	wake_scheduler(t_schedule *s)
{
	pthread_mutex_lock(&s->priority_lock);
	pthread_cond_broadcast(&s->turn_cond);
	pthread_mutex_unlock(&s->priority_lock);
}