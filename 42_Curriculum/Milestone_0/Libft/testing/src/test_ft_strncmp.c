#include <stdio.h>
#include <string.h>
#include "../../libft.h"

static void	run_test(const char *s1, const char *s2, size_t n)
{
	int	std;
	int	ft;

	std = strncmp(s1, s2, n);
	ft = ft_strncmp(s1, s2, n);

	printf("s1: \"%s\"\n", s1);
	printf("s2: \"%s\"\n", s2);
	printf("n : %zu\n", n);

	printf("strncmp:    %d\n", std);
	printf("ft_strncmp: %d\n", ft);

	if ((std == 0 && ft != 0)
		|| (std < 0 && ft >= 0)
		|| (std > 0 && ft <= 0))
		printf("SIGN MISMATCH\n");
	printf("-----------------------------\n");
}

int	main(void)
{
	run_test("abc", "abc", 3);
	run_test("abc", "abd", 3);
	run_test("abd", "abc", 3);
	run_test("abc", "abc", 0);
	run_test("abc", "abc", 2);

	run_test("abc", "abcd", 4);
	run_test("abcd", "abc", 4);
	run_test("abc", "abcd", 3);
	run_test("abcd", "abc", 3);

	run_test("", "", 1);
	run_test("", "a", 1);
	run_test("a", "", 1);

	run_test("hello", "hello", 10);
	run_test("hello", "hellp", 5);
	run_test("hellp", "hello", 5);

	run_test("test\200", "test\0", 5);
	run_test("test\377", "test\0", 5);
	run_test("a", "b", 1);
	run_test("b", "a", 1);

	run_test("abcdef", "abcxyz", 3);
	run_test("abcdef", "abcxyz", 4);
	run_test("abc", "abd", 1);

	return (0);
}