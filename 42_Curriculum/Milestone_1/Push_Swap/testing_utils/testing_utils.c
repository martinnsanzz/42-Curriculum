#include "../push_swap.h"
#include <stdio.h>

#define GREEN "\033[1;92m"
#define RED "\033[1;91m"
#define YELLOW "\033[1;93m"
#define RESET "\033[0m"

void	print_stack_int_base(t_list *stack, char *name, char *base)
{
	int		len;
	int		i;

	len = 0;
	i = 0;
	ft_printf("%s\n", name);
	if (stack)
		ft_printf("|");
	while (stack != NULL)
	{
		ft_printf("[%d]-", *(int *)stack->content);
		ft_putnbr_base((unsigned long)(*(int *)stack->content), base, &len);
		ft_printf(" | ");
		stack = stack->next;
		i++;
	}
	ft_printf("\n\n");
}

void	print_stack(t_list *stack, char *name)
{
	ft_printf("'%s': ", name);
	while (stack != NULL)
	{
		ft_printf("%d ", *(int *)stack->content);
		stack = stack->next;
	}
	ft_printf("\n");
}

void	print_lst(int *lst, int n_elements, char *msg)
{
	size_t	i;

	i = 0;
	ft_printf("%s\n", msg);
	while (i < (size_t)n_elements)
		ft_printf("[%d] ", lst[i++]);
	ft_printf("\n\n");
}

void	print_flags(t_flags flags)
{
	ft_printf("==== FLAGS ====\n");
	ft_printf("Number of flags: %d\n", flags.n_flags);
	ft_printf("Strategy: %d\n", flags.user_strat);
	ft_printf("Strategy: %d\n", flags.sys_strat);
	ft_printf("Bench mode: %d\n", flags.bench);
	ft_printf("===============\n");
	ft_printf("\n\n");
}

void	print_result(t_list *stack_a)
{
	if (compute_disorder(stack_a) == 0)
		ft_printf(GREEN"\nList is sorted !!\n"RESET);
	else
		ft_printf(RED"\nList is not sorted !!\n"RESET);
}
