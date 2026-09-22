#include "../push_swap.h"

static int	find_min(t_list	*stack);

/**
 * @brief	Sorts stack_a using a selection sort strategy. It repeatedly finds
 * 			the minimum value, rotates it to tje top, pushes tp stack_b, then
 * 			pushes stack_b back into stack_a in sorted order.
 * 
 * @details	On each iteration, the position of the current minimum in stack_a
 * 			is found. If it is found in the first half, the stack is rotated 
 * 			forward to bring it to the top. If it is found in the second half,
 * 			it is rotated in reverse, always choosing the cheaper direction.
 * 			Once at the top, it is pushed to stack_b. After all elements are
 * 			pushed, stack_b is drained backinto stack_a with 'pa',
 * 			restoring sorted order.
 * 
 * @note	When 'min_pos_index' is 0, the minimum is already at the top and
 * 			no rotation is needed before pushing. The threshold 'stack_size / 2'
 * 			ensures the cheapest rotation direction is always chosen.
 */
void	simple_strat(t_list **stack_a, t_list **stack_b,
				t_operations *op, t_flags *flags)
{
	int	min_pos_index;
	int	stack_size;

	set_flags(flags, SIMPLE);
	while (*stack_a != NULL)
	{
		stack_size = ft_lstsize(*stack_a);
		min_pos_index = find_min(*stack_a);
		if (min_pos_index < stack_size / 2)
			while (min_pos_index--)
				ra(stack_a, op);
		else if (min_pos_index != 0)
		{
			min_pos_index = stack_size - min_pos_index;
			while (min_pos_index--)
				rra(stack_a, op);
		}
		pb(stack_b, stack_a, op);
	}
	while (*stack_b != NULL)
		pa(stack_a, stack_b, op);
}

/**
 * @brief	Find the index of the node holding the minimum integer
 * 			value in a linked list.
 * 
 * @details	Traverses the entire list once, tracking the node with the lowest
 * 			integer value by dereferencing each node's pointer as 'int *'.
 * 			The position index is zero_based and increments
 * 			with each node visited.
 * 
 * @return	Zero-based index of the node containing the minimum integer value.
 * 			Returns 0 if the list is NULL or contains a single node.
 */
static int	find_min(t_list	*stack)
{
	t_list	*min_node;
	int		node_pos;
	int		min_pos_index;

	node_pos = 0;
	min_pos_index = 0;
	min_node = stack;
	while (stack != NULL)
	{
		if (*(int *)stack->content < *(int *)min_node->content)
		{
			min_node = stack;
			min_pos_index = node_pos;
		}
		node_pos += 1;
		stack = stack->next;
	}
	return (min_pos_index);
}
