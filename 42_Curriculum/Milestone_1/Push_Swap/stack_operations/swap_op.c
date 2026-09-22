#include "../push_swap.h"

static void	swap_cmd(t_list **stack)
{
	int	temp;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	temp = *(int *)(*stack)->content;
	*(int *)(*stack)->content = *(int *)(*stack)->next->content;
	*(int *)(*stack)->next->content = temp;
}

/**
 * @brief Swap the first two elements at the top of stack a. 
 * 		  Do nothing if there is only one or no elements.
 * @note  Adds 1 to the operation count.
 */
void	sa(t_list **stack_a, t_operations *op)
{
	swap_cmd(stack_a);
	op->sa += 1;
	ft_printf("sa\n");
}

/**
 * @brief Swap the first two elements at the top of stack b.
 * 		  Do nothing if there is only one or no elements.
 * @note  Adds 1 to the operation count.
 */
void	sb(t_list **stack_b, t_operations *op)
{
	swap_cmd(stack_b);
	op->sb += 1;
	ft_printf("sb\n");
}

/**
 * @brief sa and sb at the same time.
 * @note  Adds 1 to the operation count.
 */
void	ss(t_list **stack_a, t_list **stack_b, t_operations *op)
{
	swap_cmd(stack_a);
	swap_cmd(stack_b);
	op->ss += 1;
	ft_printf("ss\n");
}
