#include "../push_swap.h"

static void	push_cmd(t_list **src, t_list **dst)
{
	t_list	*temp;

	if (!src || !*src)
		return ;
	temp = *src;
	*src = (*src)->next;
	temp->next = *dst;
	*dst = temp;
}

/**
 * @brief Take the first element at the top of b and put it at the top of a.
 * 		  Do nothing if b is empty.
 * @note  Adds 1 to the operation count.
 */
void	pa(t_list **stack_a, t_list **stack_b, t_operations *op)
{
	push_cmd(stack_b, stack_a);
	op->pa += 1;
	ft_printf("pa\n");
}

/**
 * @brief Take the first element at the top of a and put it at the top of b.
 * 		  Do nothing if a is empty.
 * @note  Adds 1 to the operation count.
 */
void	pb(t_list **stack_b, t_list **stack_a, t_operations *op)
{
	push_cmd(stack_a, stack_b);
	op->pb += 1;
	ft_printf("pb\n");
}
