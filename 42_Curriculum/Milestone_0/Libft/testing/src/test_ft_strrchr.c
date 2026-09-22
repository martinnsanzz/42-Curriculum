#include <stdio.h>
#include <string.h>
#include "../../libft.h"

static void	run_test(const char *s, int c)
{
	char	*std;
	char	*ft;

	std = strrchr(s, c);
	ft = ft_strrchr(s, c);

	printf("String: \"%s\" | c: %d", s, c);
	if (c >= 32 && c <= 126)
		printf(" ('%c')", c);
	printf("\n");

	printf("strrchr:    %p", (void *)std);
	if (std)
		printf(" -> \"%s\"", std);
	printf("\n");

	printf("ft_strrchr: %p", (void *)ft);
	if (ft)
		printf(" -> \"%s\"", ft);
	printf("\n");

	if (std != ft)
		printf("POINTER MISMATCH\n");

	printf("--------------------------------\n");
}

int	main(void)
{
	run_test("hello", 'h');
	run_test("hello", 'l');
	run_test("hello", 'o');
	run_test("hello", 'x');

	run_test("aaaaa", 'a');
	run_test("abcabcabc", 'a');
	run_test("abcabcabc", 'c');

	run_test("", '\0');
	run_test("hello", '\0');

	run_test("hello world", ' ');
	run_test("123123123", '2');

	run_test("abc", 0);
	run_test("abc", 127);
	run_test("abc", 200);

	return (0);
}