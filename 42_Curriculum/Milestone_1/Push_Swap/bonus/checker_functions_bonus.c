#include "push_swap_bonus.h"

static void sort_stack(t_list **stack_a, t_list **stack_b, char *action);

/**
 * @brief  Reads push_swap operation strings from stdin and applies them to the stacks.
 *
 * @details  Calls get_next_line() in a loop to read one operation per line from
 *           file descriptor 0. Each line is dispatched to sort_stack() for
 *           execution, then freed. The loop terminates when get_next_line()
 *           returns NULL (EOF or read error). The trailing free(line) after
 *           the loop is unreachable dead code — line is always NULL at that point.
 */
void	read_input(t_list **stack_a, t_list **stack_b)
{
    char  *line;

    while (1)
    {
        line = get_next_line(0);
        if (line == NULL)
			break ;
        sort_stack(stack_a, stack_b, line);
        free(line);
    }
}

/**
 * @brief  Dispatches a single push_swap operation string to its handler function.
 *
 * @details  Compares the action string against all valid push_swap operation
 *           names (including their trailing newline). On a match, calls the
 *           corresponding stack operation function. Unrecognized strings are
 *           silently ignored. The function performs no stack modification of
 *           its own — it is a pure dispatcher.
 */
static void sort_stack(t_list **stack_a, t_list **stack_b, char *action)
{
	if (!action)
		return;
	if (ft_strcmp(action, "sa\n") == 0)
		sa(stack_a);
	else if (ft_strcmp(action, "sb\n") == 0)
		sb(stack_b);
	else if (ft_strcmp(action, "ss\n") == 0)
		ss(stack_a, stack_b);
	else if (ft_strcmp(action, "pa\n") == 0)
		pa(stack_a, stack_b);
	else if (ft_strcmp(action, "pb\n") == 0)
		pb(stack_b, stack_a);
	else if (ft_strcmp(action, "ra\n") == 0)
		ra(stack_a);
	else if (ft_strcmp(action, "rb\n") == 0)
		rb(stack_b);
	else if (ft_strcmp(action, "rr\n") == 0)
		rr(stack_a, stack_b);
	else if (ft_strcmp(action, "rra\n") == 0)
		rra(stack_a);
	else if (ft_strcmp(action, "rrb\n") == 0)
		rrb(stack_b);
	else if (ft_strcmp(action, "rrr\n") == 0)
		rrr(stack_a, stack_b);
}
