#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include <stdbool.h>

typedef enum e_strategy
{
	ADAPTIVE,
	SIMPLE,
	MEDIUM,
	COMPLEX
}	t_strategy;

typedef struct s_flags
{
	t_strategy	user_strat;
	t_strategy	sys_strat;
	bool		bench;
	int			n_flags;
}	t_flags;

typedef struct s_operations
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
}	t_operations;

//Argument validation functions
int		check_argv(int argc, char *argv[], int **unsorted_lst, t_flags *flags);

//Utils functions
void	print_error(void);
long	ft_atoi_strict(const char *nptr);
int		ft_strcmp(const char *s1, const char *s2);
int		sum_operations(t_operations op);
void	set_flags(t_flags *flags, int flag);

//Maths utils functions
int		ft_sqrt(int nb);

//TO BE DELETED
//Printing functions to test
void	print_stack(t_list *stack, char *name);
void	print_flags(t_flags flags);
void	print_lst(int *lst, int n_elements, char *msg);
void	print_result(t_list *stack_a);
void	print_stack_int_base(t_list *stack, char *name, char *base);

//Actions functions
void	sa(t_list **stack_a, t_operations *op);
void	sb(t_list **stack_b, t_operations *op);
void	ss(t_list **stack_a, t_list **stack_b, t_operations *op);

void	pa(t_list **stack_a, t_list **stack_b, t_operations *op);
void	pb(t_list **stack_b, t_list **stack_a, t_operations *op);

void	ra(t_list **stack_a, t_operations *op);
void	rb(t_list **stack_b, t_operations *op);
void	rr(t_list **stack_a, t_list **stack_b, t_operations *op);

void	rra(t_list **stack_a, t_operations *op);
void	rrb(t_list **stack_b, t_operations *op);
void	rrr(t_list **stack_a, t_list **stack_b, t_operations *op);

//Alghoritm functions
void	bench(t_flags flags, t_operations op, float disorder);
void	select_strategy(t_list **stack_a, t_list **stack_b, t_flags *flags);
void	simple_strat(t_list **stack_a, t_list **stack_b,
			t_operations *op, t_flags *flags);
void	medium_strat(t_list **stack_a, t_list **stack_b,
			t_operations *op, t_flags *flags);
void	complex_strat(t_list **stack_a, t_list **stack_b,
			t_operations *op, t_flags *flags);
float	compute_disorder(t_list *stack_a);

#endif