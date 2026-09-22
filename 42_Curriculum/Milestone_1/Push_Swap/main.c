#include "push_swap.h"

static void	init_flags(t_flags *flags);
static void	normalize(int **lst, int n_elements);
static void	linked_lst_creation(t_list **stack, int *lst, int n_elements);

int	main(int argc, char *argv[])
{
	t_flags			flags;
	t_list			*stack_a;
	t_list			*stack_b;
	int				*tmp_lst;
	int				n_elements;

	init_flags(&flags);
	stack_b = NULL;
	n_elements = check_argv(argc, argv, &tmp_lst, &flags);
	if (!tmp_lst || argc < (3 + flags.n_flags))
		return (0);
	normalize(&tmp_lst, n_elements);
	linked_lst_creation(&stack_a, tmp_lst, n_elements);
	select_strategy(&stack_a, &stack_b, &flags);
	ft_lstclear(&stack_a, (*free));
	ft_lstclear(&stack_b, (*free));
	free(tmp_lst);
	return (0);
}

/**
 * @brief Initializes flags to default states:
 * 			- Number of flags = 0
 * 			- Bench mode = 0 (False / Off)
 * 			- Strategy = Adaptive / 0
 */
static void	init_flags(t_flags *flags)
{
	flags->n_flags = 0;
	flags->bench = false;
	flags->user_strat = ADAPTIVE;
	flags->sys_strat = ADAPTIVE;
}

void	set_flags(t_flags *flags, int flag)
{
	if (flags->sys_strat == ADAPTIVE)
		flags->user_strat = flag;
	else
		flags->user_strat = flags->sys_strat;
}

/**
 * @brief  Builds a linked list from a normalized integer array.
 *
 * @details  Iterates over `normalize_lst`, allocating a new `int` on the heap 
 * 			 for each element and wrapping it in a `t_list` node 
 * 			 via `ft_lstnew`. 
 * 			 Each node is appended to `*link_lst` via `ft_lstadd_back`. 
 * 			 On `ft_lstnew` failure, `ft_lstclear` is called to free the lis
 *
 * @param  link_lst       t_list** — Output parameter. Set to NULL before 
 * 									 iteration, then built up node by node.
 * @param  lst  int*     — The source array of normalized integer values.
 * @param  n_elements     int      — Number of elements to read from `lst`
 *                                   and insert into the list.
 */
static void	linked_lst_creation(t_list **link_lst, int *lst, int n_elements)
{
	int		i;
	int		*content;
	t_list	*node;

	*link_lst = NULL;
	i = 0;
	while (i < n_elements)
	{
		content = ft_calloc(1, sizeof(int));
		if (!content)
			return ;
		*content = lst[i];
		node = ft_lstnew(content);
		if (!node)
		{
			ft_lstclear(link_lst, (*free));
			return ;
		}
		ft_lstadd_back(link_lst, node);
		i++;
	}
}

/**
 * @brief  Replaces an integer array with its normalized-rank equivalent.
 *
 * @details  For each element at index `i`, counts how many other elements in the
 *           array are smaller than it — this count becomes its normalized rank.
 *           The result is stored in a newly allocated array. Once all ranks are
 *           computed, the original array is freed and `*lst` is updated to point
 *           to the normalized array.
 *
 * @param  lst        int** — Pointer to the integer array to normalize.
 * @param  n_elements int   — The number of elements in `*lst`.
 */
static void	normalize(int **lst, int n_elements)
{
	int		*normalized_lst;
	int		total_min_val;
	size_t	i;
	size_t	j;

	if (!*lst || n_elements < 0)
		return ;
	normalized_lst = ft_calloc(n_elements, sizeof(int));
	if (!normalized_lst)
		return ;
	i = 0;
	while (i < (size_t)n_elements)
	{
		j = 0;
		total_min_val = 0;
		while (j < (size_t)n_elements)
		{
			if ((*lst)[j] < (*lst)[i])
				total_min_val++;
			j++;
		}
		normalized_lst[i++] = total_min_val;
	}
	free(*lst);
	*lst = normalized_lst;
}
