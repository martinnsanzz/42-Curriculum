#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

static void	run_test(const char *s1, const char *s2, const char *expected)
{
	char	*res;

	res = ft_strjoin(s1, s2);

	printf("s1: \"%s\"\n", s1);
	printf("s2: \"%s\"\n", s2);

	printf("expected: \"%s\"\n", expected);
	printf("result:   \"%s\"\n", res ? res : "NULL");

	if (!res)
		printf("TEST FAILED (NULL)\n");
	else if (strcmp(res, expected) != 0)
		printf("TEST FAILED (content mismatch)\n");
	else
		printf("TEST OK\n");

	if (res == s1 || res == s2)
		printf("ERROR: returned original pointer\n");

	/* null termination check */
	if (res)
	{
		size_t i = 0;
		while (res[i])
			i++;
		if (i != strlen(res))
			printf("ERROR: not properly null terminated\n");
	}

	printf("-----------------------------\n");

	free(res);
}

int	main(void)
{
	run_test("hello", "world", "helloworld");
	run_test("hello ", "world", "hello world");
	run_test("", "world", "world");
	run_test("hello", "", "hello");
	run_test("", "", "");

	run_test("a", "b", "ab");
	run_test("42", " school", "42 school");

	run_test("long_string_", "test_case", "long_string_test_case");

	return (0);
}