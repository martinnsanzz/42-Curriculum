/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_scan.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 13:08:09 by 2002mssm02        #+#    #+#             */
/*   Updated: 2026/10/05 13:52:59 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Restore the min-heap property from index i upwards.
 *
 * @param s Scheduler owning the heap.
 * @param i Index to start from.
 */
static void	sift_up(t_schedule *s, size_t i)
{
	size_t	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (s->heap[parent].key <= s->heap[i].key)
			break ;
		heap_swap(&s->heap[parent], &s->heap[i]);
		i = parent;
	}
}

/**
 * @brief Remove the entry at index i, keeping the min-heap property.
 *
 * Moves the last entry into the hole, then sifts it down or up (only
 * one of the two moves anything). Caller must hold priority_lock.
 *
 * @param s Scheduler owning the heap.
 * @param i Index of the entry to remove.
 */
void	heap_remove_at(t_schedule *s, size_t i)
{
	s->heap_size--;
	if (i == s->heap_size)
		return ;
	s->heap[i] = s->heap[s->heap_size];
	sift_down(s, i);
	sift_up(s, i);
}

/**
 * @brief Tell whether request a is served before request b.
 *
 * Smaller key first. Equal keys fall back to arrival order (seq).
 */
static bool	is_before(t_request *a, t_request *b)
{
	return (a->key < b->key || (a->key == b->key && a->seq < b->seq));
}

/**
 * @brief Find the best waiting coder that can take its dongles now.
 *
 * Among the coders whose dongles are free and out of cooldown, returns
 * the one served first by (key, seq). If none can go, *wake_at is the
 * earliest cooldown end of any waiting coder, or 0 if only a release
 * can help. Caller must hold priority_lock.
 *
 * @param s       Scheduler owning the heap.
 * @param wake_at Output: 0 = wait for a release, else absolute ms.
 *
 * @return Heap index of the coder to grant, or -1.
 */
int	find_grantable(t_schedule *s, size_t *wake_at)
{
	size_t	i;
	size_t	t;
	int		best;

	*wake_at = 0;
	best = -1;
	i = 0;
	while (i < s->heap_size)
	{
		if (can_grant(s->heap[i].coder, &t))
		{
			if (best < 0 || is_before(&s->heap[i], &s->heap[best]))
				best = (int)i;
		}
		else if (t != 0 && (*wake_at == 0 || t < *wake_at))
			*wake_at = t;
		i++;
	}
	return (best);
}
