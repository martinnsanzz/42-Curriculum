#include "../push_swap.h"

static int	max_bits(t_list *stack);

/**
 * @brief  Sorts stack_a using a radix sort algorithm over all bits of the
 *         maximum value.
 *
 * @details  Iterates over each bit position (up to `max_bits` iterations). 
 * 			 On each pass, every element in stack_a is inspected at bit 
 * 			 position `i`: elements with bit `i` unset or 0 are pushed 
 * 			 to stack_b, elements with bit `i` set  or 1 are rotated
 *           to the bottom of stack_a. Once all elements are processed,
 *           the entirety of stack_b is pushed back to stack_a. After all
 *           bit passes, stack_a is sorted in ascending order.

 * @note  Correctness depends entirely on values being normalized to
 *        [0, stack_size - 1] before this function is called. Non-normalized
 *        values will produce incorrect bit inspection and a wrong sort order.
 *        stack_b is expected to be empty on entry; leftover elements will
 *        be pushed back into stack_a and corrupt the result.
 */
void	complex_strat(t_list **stack_a, t_list **stack_b,
				t_operations *op, t_flags *flags)
{
	int	stack_size;
	int	restore;
	int	i;
	int	bits;

	set_flags(flags, COMPLEX);
	bits = max_bits(*stack_a);
	i = 0;
	while (i < bits)
	{
		stack_size = ft_lstsize(*stack_a);
		while (stack_size > 0)
		{
			if (((*(int *)(*stack_a)->content >> i) & 1) == 0)
				pb(stack_b, stack_a, op);
			else
				ra(stack_a, op);
			stack_size--;
		}
		restore = ft_lstsize(*stack_b);
		while (restore--)
			pa(stack_a, stack_b, op);
		i++;
	}
}

/**
 * @brief  Computes the number of bits required to represent the maximum
 *         value present in the stack.
 *
 * @details  Determines the stack size via `ft_lstsize`, then treats
 *           `stack_size - 1` as the maximum possible value. Counts the
 *           number of bit-shift operations needed to reduce that value to
 *           zero, effectively computing `floor(log2(max_value)) + 1`.
 *
 * @return int — The bit-width of `stack_size - 1`.
 *               Returns 0 if the stack is empty or contains one element.
 *               Returns 1 if stack_size == 2, 2 if stack_size <= 4, etc.
 *
 * @note  The result is only meaningful when stack values have been
 *        normalized to the range [0, stack_size - 1] beforehand.
 *        Calling this on a non-normalized stack will produce a bit count
 *        inconsistent with the actual content.
 */
static int	max_bits(t_list *stack)
{
	int	stack_size;
	int	max_value;
	int	bits;

	if (!stack)
		return (0);
	stack_size = ft_lstsize(stack);
	max_value = stack_size - 1;
	bits = 0;
	while (max_value)
	{
		bits++;
		max_value >>= 1;
	}
	return (bits);
}
