#include <stdio.h>
#include <stdlib.h>
#include "../../libft.h"

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

static t_list	*create_node(int value)
{
	int		*data;
	t_list	*node;

	data = malloc(sizeof(int));
	*data = value;
	node = ft_lstnew(data);
	return (node);
}

static void	test_empty_list(void)
{
	t_list	*lst;
	t_list	*new;

	printf("TEST: empty list\n");

	lst = NULL;
	new = create_node(42);

	ft_lstadd_front(&lst, new);

	print_list(lst);

	if (lst != new)
		printf("ERROR: new node not first element\n");

	free(lst->content);
	free(lst);
}

static void	test_existing_list(void)
{
	t_list	*lst;
	t_list	*new;

	printf("TEST: existing list\n");

	lst = create_node(1);
	lst->next = create_node(2);

	new = create_node(99);

	ft_lstadd_front(&lst, new);

	print_list(lst);

	if (*(int *)lst->content != 99)
		printf("ERROR: new node not inserted at front\n");

	if (*(int *)lst->next->content != 1)
		printf("ERROR: old head not shifted\n");

	while (lst)
	{
		t_list *tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

int	main(void)
{
	test_empty_list();
	test_existing_list();
	return (0);
}