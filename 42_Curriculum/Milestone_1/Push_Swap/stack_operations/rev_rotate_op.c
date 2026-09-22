#include "../push_swap.h"

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
 * @note  Adds 1 to the operation count.
 */
void	rra(t_list **stack_a, t_operations *op)
{
	rev_rotate_cmd(stack_a);
	op->rra += 1;
	ft_printf("rra\n");
}

/**
 * @brief Shift down all elements of stack b by one.
 * 		  The last element becomes the first one.
 * @note  Adds 1 to the operation count.
 */
void	rrb(t_list **stack_b, t_operations *op)
{
	rev_rotate_cmd(stack_b);
	op->rrb += 1;
	ft_printf("rrb\n");
}

/**
 * @brief rra and rrb at the same time.
 * @note  Adds 1 to the operation count.
 */
void	rrr(t_list **stack_a, t_list **stack_b, t_operations *op)
{
	rev_rotate_cmd(stack_a);
	rev_rotate_cmd(stack_b);
	op->rrr += 1;
	ft_printf("rrr\n");
}
