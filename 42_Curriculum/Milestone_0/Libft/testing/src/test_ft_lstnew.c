#include <stdio.h>
#include <stdlib.h>
#include "../../libft.h"

static void	test_string(void)
{
	char	*str = "hello";
	t_list	*node;

	node = ft_lstnew(str);

	printf("TEST STRING\n");
	printf("node:   %p\n", (void *)node);
	printf("content:%p -> \"%s\"\n", node->content, (char *)node->content);
	printf("next:   %p\n", (void *)node->next);

	if (node->content != str)
		printf("CONTENT POINTER MISMATCH\n");

	if (node->next != NULL)
		printf("NEXT SHOULD BE NULL\n");

	printf("------------------------\n");

	free(node);
}

static void	test_int(void)
{
	int		value = 42;
	t_list	*node;

	node = ft_lstnew(&value);

	printf("TEST INT\n");
	printf("node:   %p\n", (void *)node);
	printf("content:%p -> %d\n", node->content, *(int *)node->content);
	printf("next:   %p\n", (void *)node->next);

	if (*(int *)node->content != 42)
		printf("CONTENT VALUE MISMATCH\n");

	if (node->next != NULL)
		printf("NEXT SHOULD BE NULL\n");

	printf("------------------------\n");

	free(node);
}

static void	test_null_content(void)
{
	t_list	*node;

	node = ft_lstnew(NULL);

	printf("TEST NULL CONTENT\n");
	printf("node:   %p\n", (void *)node);
	printf("content:%p\n", node->content);
	printf("next:   %p\n", (void *)node->next);

	if (node->content != NULL)
		printf("CONTENT SHOULD BE NULL\n");

	if (node->next != NULL)
		printf("NEXT SHOULD BE NULL\n");

	printf("------------------------\n");

	free(node);
}

int	main(void)
{
	test_string();
	test_int();
	test_null_content();
	return (0);
}