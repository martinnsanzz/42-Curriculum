#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

static void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static int	count_words(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static void	print_split(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		printf("[%d]: \"%s\"\n", i, arr[i]);
		i++;
	}
	printf("[%d]: NULL\n", i);
}

static void	run_test(const char *s, char c,
					const char **expected, int expected_count)
{
	char	**res;
	int		i;

	printf("input: \"%s\" | delim: '%c'\n", s, c);

	res = ft_split(s, c);

	if (!res)
	{
		printf("ft_split returned NULL\n");
		printf("-----------------------------\n");
		return ;
	}

	print_split(res);

	if (count_words(res) != expected_count)
		printf("ERROR: wrong word count\n");

	i = 0;
	while (expected[i])
	{
		if (!res[i] || strcmp(res[i], expected[i]) != 0)
			printf("ERROR: mismatch at index %d\n", i);
		i++;
	}

	if (res[i] != NULL)
		printf("ERROR: array not NULL-terminated\n");

	printf("-----------------------------\n");

	free_split(res);
}

int	main(void)
{
	const char *t1[] = {"hello", "world", "this", "is", "martin", NULL};
	const char *t2[] = {"42", "school", "libft", NULL};
	const char *t3[] = {"single", NULL};
	const char *t4[] = {"a", "b", "c", NULL};
	const char *t5[] = {NULL};
	const char *t6[] = {"no_delimiters", NULL};

	run_test("hello world this is martin", ' ', t1, 5);
	run_test("42:school:libft", ':', t2, 3);
	run_test("single", ' ', t3, 1);
	run_test("a,b,c", ',', t4, 3);

	run_test("", ' ', t5, 0);
	run_test("     ", ' ', t5, 0);

	run_test("no_delimiters", ',', t6, 1);

	run_test("  hello   world  this   is   martin  ", ' ', t1, 5);
	run_test(",,,a,,b,c,,", ',', t4, 3);

	return (0);
}