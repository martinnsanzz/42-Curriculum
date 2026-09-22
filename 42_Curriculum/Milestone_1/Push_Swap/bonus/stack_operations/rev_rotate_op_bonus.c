#include "../push_swap_bonus.h"

static void	rev_rotate_cmd(t_list **stack)
{
	t_list	*last;
	t_list	*sec_last;

	if (!stack || !(*stack)->next)
		return ;
	last = ft_lstlast(*stack);
	sec_last = *stack;
	while (sec_last->next != last)
		sec_last = sec_last->next;
	last->next = (*stack);
	sec_last->next = NULL;
	*stack = last;
}

/**
 * @brief Shift down all elements of stack a by one.
 * 		  The last element becomes the first one.
 */
void	rra(t_list **stack_a)
{
	rev_rotate_cmd(stack_a);
}

/**
 * @brief Shift down all elements of stack b by one.
 * 		  The last element becomes the first one.
 */
void	rrb(t_list **stack_b)
{
	rev_rotate_cmd(stack_b);
}

/**
 * @brief rra and rrb at the same time.
 */
void	rrr(t_list **stack_a, t_list **stack_b)
{
	rev_rotate_cmd(stack_a);
	rev_rotate_cmd(stack_b);
}
