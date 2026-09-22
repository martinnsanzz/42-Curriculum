#include "../push_swap.h"

static void	rotate_cmd(t_list **stack)
{
	t_list	*first;
	t_list	*last;

	if (!stack || !(*stack)->next)
		return ;
	first = *stack;
	*stack = (*stack)->next;
	last = ft_lstlast(*stack);
	last->next = first;
	first->next = NULL;
}

/**
 * @brief Shift up all elements of stack a by one.
 * 		  The first element becomes the last one.
 * @note  Adds 1 to the operation count.
 */
void	ra(t_list **stack_a, t_operations *op)
{
	rotate_cmd(stack_a);
	op->ra += 1;
	ft_printf("ra\n");
}

/**
 * @brief Shift up all elements of stack b by one.
 * 		  The first element becomes the last one.
 * @note  Adds 1 to the operation count.
 */
void	rb(t_list **stack_b, t_operations *op)
{
	rotate_cmd(stack_b);
	op->rb += 1;
	ft_printf("rb\n");
}

/**
 * @brief ra and rb at the same time.
 * @note  Adds 1 to the operation count.
 */
void	rr(t_list **stack_a, t_list **stack_b, t_operations *op)
{
	rotate_cmd(stack_a);
	rotate_cmd(stack_b);
	op->rr += 1;
	ft_printf("rr\n");
}
