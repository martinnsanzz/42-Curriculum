#include "../push_swap.h"
#include <stdio.h>

static bool	find_node(t_list *stack, int *index, int cur_chunk, int chunk_size);
static int	find_max(t_list	*stack);
static void	push_sorted_to_a(t_list **stack_b, t_list **stack_a,
				t_operations *op);

/**
 * @brief  Sorts stack_a using a chunk-based strategy. It sorts the values
 *         into chunks, pushes them to stack_b and sends back the values sorted.
 *
 * @details  Computes the number of chunks as the integer square root of the
 *           stack size `+ 1 if the size is not a perfect square`, then
 * 			 calculates the size of the chunks using ceiling division. 
 * 			 Iterates through each chunk in order: for every node
 *           in stack_a belonging to the current chunk, rotates it to the top and
 *           pushes it to stack_b. Once all chunks are processed, calls
 *           `push_sorted_to_a` to drain stack_b back into stack_a in sorted 
 * 			 order.

 * @note  The ceiling division `(stack_size + total_chunks - 1) / total_chunks`
 *        ensures no values fall outside the last chunk due to rounding.
 *        `node_index` is reset to 0 before each `find_node` call so the search
 *        always restarts from the head of the current stack_a state.
 */
void	medium_strat(t_list **stack_a, t_list **stack_b,
				t_operations *op, t_flags *flags)
{
	int	total_chunks;
	int	chunk_size;
	int	stack_size;
	int	curr_chunk;
	int	node_index;

	set_flags(flags, MEDIUM);
	stack_size = ft_lstsize(*stack_a);
	total_chunks = ft_sqrt(stack_size);
	chunk_size = (stack_size + total_chunks - 1) / total_chunks;
	curr_chunk = 0;
	while (curr_chunk < total_chunks)
	{
		while (find_node(*stack_a, &node_index, curr_chunk, chunk_size))
		{
			if (node_index != 0)
				while (node_index--)
					ra(stack_a, op);
			pb(stack_b, stack_a, op);
			node_index = 0;
		}
		curr_chunk++;
	}
	push_sorted_to_a(stack_b, stack_a, op);
}

/**
 * @brief  Searches a stack for the first node whose value belongs to a
 *         given chunk.
 *
 * @details  Iterates the list from head to tail. For each node, checks whether
 *           the integer value falls within the chunk.
 *           On the first match, writes the zero-based position to `*index` and
 *           returns true. If no node matches, `*index` is left unmodified and
 *           false is returned.
 *
 * @return bool — true if a matching node was found, false otherwise.
 *
 * @note  Only the first matching node is returned. If multiple nodes belong
 *        to the same chunk, subsequent ones are ignored. User needs to handle
 * 		  loop through the whole list to find all nodes of the chunk
 */
static bool	find_node(t_list *stack, int *index, int cur_chunk, int chunk_size)
{
	int	node_pos;

	if (chunk_size <= 0)
		return (false);
	node_pos = 0;
	while (stack != NULL)
	{
		if ((*(int *)stack->content) / chunk_size == cur_chunk)
		{
			*index = node_pos;
			return (true);
		}
		node_pos += 1;
		stack = stack->next;
	}
	return (false);
}

/**
 * @brief  Drains stack_b into stack_a by repeatedly pushing the current
 *         maximum value to the top, then transferring it.
 *
 * @details  Iterates exactly `ft_lstsize(*stack_b)` times at entry — the count
 *           is fixed before the loop begins. On each iteration, the index of the
 *           current maximum in stack_b is found, the list is rotated that many
 *           times to bring it to the top, pushed to stack_a via `pa`, then
 *           reverse-rotated by the same count to restore the relative order of
 *           remaining elements. After all iterations, stack_b is empty and
 *           stack_a receives elements in descending order of value.
 */
static void	push_sorted_to_a(t_list **stack_b, t_list **stack_a,
					t_operations *op)
{
	int	stack_size;
	int	max_pos;
	int	restore;

	stack_size = ft_lstsize(*stack_b);
	while (stack_size--)
	{
		max_pos = find_max(*stack_b);
		restore = max_pos;
		if (max_pos != 0)
			while (max_pos--)
				rb(stack_b, op);
		pa(stack_a, stack_b, op);
		if (restore != 0)
			while (restore--)
				rrb (stack_b, op);
	}
}

/**
 * @brief  Finds the index of the node holding the maximum integer value 
 * 		   in a linked list.
 *
 * @details  Traverses the entire list once, tracking the node with the
 * 			 highest integer value by dereferencing each node's `content`
 * 			 pointer as `int *`. The position index is zero-based and 
 * 			 increments with each node visited.
 *
 * @return int — Zero-based index of the node containing the maximum 
 * 				 integer value.
 *               Returns 0 if the list is NULL or contains a single node.
 */
static int	find_max(t_list	*stack)
{
	t_list	*max_node;
	int		node_pos;
	int		max_pos_index;

	if (!stack)
		return (-1);
	node_pos = 0;
	max_pos_index = 0;
	max_node = stack;
	while (stack != NULL)
	{
		if (*(int *)stack->content > *(int *)max_node->content)
		{
			max_node = stack;
			max_pos_index = node_pos;
		}
		node_pos += 1;
		stack = stack->next;
	}
	return (max_pos_index);
}
