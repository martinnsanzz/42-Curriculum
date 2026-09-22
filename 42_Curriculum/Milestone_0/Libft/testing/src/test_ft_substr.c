#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

static void	run_test(const char *s, unsigned int start, size_t len,
						const char *expected)
{
	char	*res;

	res = ft_substr(s, start, len);

	printf("input: \"%s\"\n", s);
	printf("start: %u | len: %zu\n", start, len);

	printf("expected: \"%s\"\n", expected);
	printf("result:   \"%s\"\n", res ? res : "NULL");

	if (!res && expected)
		printf("TEST FAILED (NULL result)\n");
	else if (res && strcmp(res, expected) != 0)
		printf("TEST FAILED (content mismatch)\n");
	else
		printf("TEST OK\n");

	/* check null termination manually */
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
	run_test("", 10, 5, "");
	run_test("hello", 10, 5, "");
	run_test("hello world", 0, 5, "hello");
	run_test("hello world", 6, 5, "world");
	run_test("hello world", 6, 20, "world");

	run_test("hello", 2, 2, "ll");
	run_test("hello", 4, 10, "o");

	run_test("hello", 5, 5, "");

	run_test("", 0, 5, "");

	run_test("test", 0, 0, "");
	run_test("test", 2, 0, "");

	run_test("a", 0, 1, "a");
	run_test("a", 1, 1, "");

	return (0);
}