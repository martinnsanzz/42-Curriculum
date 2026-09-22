#include <stdio.h>
#include <stdlib.h>
#include "../../libft.h"

static t_list	*create_node(int value)
{
	int		*data;
	t_list	*node;

	data = malloc(sizeof(int));
	*data = value;
	node = ft_lstnew(data);
	return (node);
}

static void	free_list(t_list *lst)
{
	t_list	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

static void	run_test(t_list *lst, int expected, const char *label)
{
	int	res;

	printf("TEST: %s\n", label);

	res = ft_lstsize(lst);

	printf("expected: %d\n", expected);
	printf("ft_lstsize: %d\n", res);

	if (res != expected)
		printf("TEST FAILED\n");
	else
		printf("TEST OK\n");

	printf("-----------------------------\n");
}

int	main(void)
{
	t_list	*lst;

	/* empty list */
	lst = NULL;
	run_test(lst, 0, "empty list");

	/* single node */
	lst = create_node(1);
	run_test(lst, 1, "single node");
	free_list(lst);

	/* two nodes */
	lst = create_node(1);
	lst->next = create_node(2);
	run_test(lst, 2, "two nodes");
	free_list(lst);

	/* five nodes */
	lst = create_node(1);
	lst->next = create_node(2);
	lst->next->next = create_node(3);
	lst->next->next->next = create_node(4);
	lst->next->next->next->next = create_node(5);

	run_test(lst, 5, "five nodes");

	free_list(lst);

	return (0);
}