#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

static void	run_test(const char *s1, const char *set, const char *expected)
{
	char	*res;

	res = ft_strtrim(s1, set);

	printf("s1: \"%s\"\n", s1);
	printf("set: \"%s\"\n", set);

	printf("expected: \"%s\"\n", expected);
	printf("result:   \"%s\"\n", res ? res : "NULL");

	if (!res)
		printf("TEST FAILED (NULL)\n");
	else if (strcmp(res, expected) != 0)
		printf("TEST FAILED (content mismatch)\n");
	else
		printf("TEST OKkkkk\n");

	if (res == s1)
		printf("ERROR: returned original pointer\n");

	/* null termination check */
	if (res)
	{
		size_t i = 0;
		while (res[i])
			i++;
		if (i != strlen(res))
			printf("ERROR: not null terminated\n");
	}

	printf("-----------------------------\n");

	free(res);
}

int	main(void)
{
	run_test("aaaaa", "a", "");
	run_test("hello", "h", "ello");
	run_test("hello", "o", "hell");
	run_test("hello", "ho", "ell");

	run_test("  hello  ", " ", "hello");
	run_test("\t\nhello\t", "\t\n", "hello");

	run_test("xxhelloxx", "x", "hello");
	run_test("abcXYZabc", "abc", "XYZ");

	run_test("aaaaa", "b", "aaaaa");

	run_test("", "a", "");
	run_test("", "", "");

	run_test("hello", "", "hello");

	run_test("ababahelloababa", "ab", "hello");

	return (0);
}