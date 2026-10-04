/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 12:52:23 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:25:04 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void		sift_down(t_schedule *s);
static size_t	smallest_child(t_schedule *s, size_t i);
static void		heap_swap(t_request *a, t_request *b);

/**
 * @brief Insert a coder into the scheduler's min-heap.
 *
 * Appends the coder at index heap_size with key = compute_key()
 * (FIFO: arrival sequence, EDF: deadline), then sifts it up
 * through parent (i - 1) / 2 until the parent key is <= its key.
 * Caller must hold priority_lock.
 *
 * @param s     Scheduler owning the heap.
 * @param coder Coder requesting dongle access.
 *
 * @return 0 on success, -1 if the heap is full.
 */
int	heap_push(t_schedule *s, t_coder *coder)
{
	size_t	i;
	size_t	parent;

	if ((int)s->heap_size == *(coder->total_coders))
		return (-1);
	i = s->heap_size;
	s->heap[i].coder = coder;
	s->heap[i].key = compute_key(s, coder);
	s->heap_size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (s->heap[parent].key <= s->heap[i].key)
			break ;
		heap_swap(&s->heap[parent], &s->heap[i]);
		i = parent;
	}
	return (0);
}

/**
 * @brief Remove and return the coder with the smallest key.
 *
 * Saves the root, moves the last entry to index 0, decrements
 * heap_size, then sifts down to restore the heap property.
 * Caller must hold priority_lock.
 *
 * @param s Scheduler owning the heap.
 *
 * @return The coder at the root, or NULL if the heap is empty.
 */
t_coder	*heap_pop(t_schedule *s)
{
	t_coder	*top;

	if (s->heap_size == 0)
		return (NULL);
	top = s->heap[0].coder;
	s->heap_size--;
	s->heap[0] = s->heap[s->heap_size];
	sift_down(s);
	return (top);
}

/**
 * @brief Swap two heap entries.
 * @param a First entry.
 * @param b Second entry.
 */
static void	heap_swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * @brief Find which of a node and its children holds the smallest key.
 *
 * Children sit at 2i + 1 and 2i + 2. Indices >= heap_size are ignored.
 *
 * @param s Scheduler owning the heap.
 * @param i Index of the parent node.
 *
 * @return Index of the smallest key among i and its children.
 *         Returns i itself if no child is smaller.
 */
static size_t	smallest_child(t_schedule *s, size_t i)
{
	size_t	left;
	size_t	right;
	size_t	min;

	left = 2 * i + 1;
	right = 2 * i + 2;
	min = i;
	if (left < s->heap_size && s->heap[left].key < s->heap[min].key)
		min = left;
	if (right < s->heap_size && s->heap[right].key < s->heap[min].key)
		min = right;
	return (min);
}

/**
 * @brief Restore the min-heap property from the root downwards.
 *
 * Swaps the root with its smallest child until the node has no
 * smaller child or becomes a leaf. Called after the root is replaced.
 *
 * @param s Scheduler owning the heap.
 */
static void	sift_down(t_schedule *s)
{
	size_t	i;
	size_t	next;

	i = 0;
	while (1)
	{
		next = smallest_child(s, i);
		if (next == i)
			break ;
		heap_swap(&s->heap[i], &s->heap[next]);
		i = next;
	}
}
