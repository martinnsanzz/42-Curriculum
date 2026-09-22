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
	t_list	*last;

	printf("TEST: %s\n", label);

	last = ft_lstlast(lst);

	if (!last)
	{
		printf("returned: NULL\n");
		if (expected != -1)
			printf("TEST FAILED\n");
		else
			printf("TEST OK\n");
	}
	else
	{
		printf("returned content: %d\n", *(int *)last->content);
		printf("expected content: %d\n", expected);

		if (*(int *)last->content != expected)
			printf("TEST FAILED\n");
		else
			printf("TEST OK\n");
	}

	printf("-----------------------------\n");
}

int	main(void)
{
	t_list	*lst;

	/* empty list */
	lst = NULL;
	run_test(lst, -1, "empty list");

	/* single node */
	lst = create_node(42);
	run_test(lst, 42, "single node");
	free_list(lst);

	/* multiple nodes */
	lst = create_node(1);
	lst->next = create_node(2);
	lst->next->next = create_node(3);
	lst->next->next->next = create_node(4);

	run_test(lst, 4, "four nodes");

	free_list(lst);

	return (0);
}