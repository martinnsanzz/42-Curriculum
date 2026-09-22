#ifndef PUSH_SWAP_BONUS_H
# define PUSH_SWAP_BONUS_H

# include "../libft/libft.h"
# include "get_next_line/get_next_line.h"

//Argument validation functions
int		check_argv(int argc, char *argv[], int **unsorted_lst);

//Utils functions
long	ft_atoi_strict(const char *nptr);
void	print_error(void);
float	compute_disorder(t_list *stack_a);
void	check_results(t_list *stack_a, t_list *stack_b);
int		ft_strcmp(const char *s1, const char *s2);

//Actions functions
void	sa(t_list **stack_a);
void	sb(t_list **stack_b);
void	ss(t_list **stack_a, t_list **stack_b);

void	pa(t_list **stack_a, t_list **stack_b);
void	pb(t_list **stack_b, t_list **stack_a);

void	ra(t_list **stack_a);
void	rb(t_list **stack_b);
void	rr(t_list **stack_a, t_list **stack_b);

void	rra(t_list **stack_a);
void	rrb(t_list **stack_b);
void	rrr(t_list **stack_a, t_list **stack_b);

//Input functions
void	read_input(t_list **stack_a, t_list **stack_b);

#endif