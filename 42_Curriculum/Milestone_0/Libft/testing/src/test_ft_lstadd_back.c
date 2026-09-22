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

static void	print_list(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		printf("node[%d] content: %d | addr: %p | next: %p\n",
			i, *(int *)lst->content, (void *)lst, (void *)lst->next);
		lst = lst->next;
		i++;
	}
	printf("-----------------------------\n");
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

static void	test_empty_list(void)
{
	t_list	*lst;
	t_list	*new;

	printf("TEST: empty list\n");

	lst = NULL;
	new = create_node(10);

	ft_lstadd_back(&lst, new);

	print_list(lst);

	if (lst != new)
		printf("ERROR: new node not set as first element\n");

	free_list(lst);
}

static void	test_single_node(void)
{
	t_list	*lst;
	t_list	*new;

	printf("TEST: single node list\n");

	lst = create_node(1);
	new = create_node(2);

	ft_lstadd_back(&lst, new);

	print_list(lst);

	if (*(int *)lst->next->content != 2)
		printf("ERROR: new node not appended\n");

	free_list(lst);
}

static void	test_multiple_nodes(void)
{
	t_list	*lst;
	t_list	*new;
	t_list	*last;

	printf("TEST: multiple node list\n");

	lst = create_node(1);
	lst->next = create_node(2);
	lst->next->next = create_node(3);

	new = create_node(99);

	ft_lstadd_back(&lst, new);

	print_list(lst);

	last = ft_lstlast(lst);

	if (*(int *)last->content != 99)
		printf("ERROR: new node not last\n");

	free_list(lst);
}

int	main(void)
{
	test_empty_list();
	test_single_node();
	test_multiple_nodes();
	return (0);
}