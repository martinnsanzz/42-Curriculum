#include <stdio.h>
#include <stdlib.h>
#include "../../libft.h"

static void	run_test(const char *s)
{
	int	std;
	int	ft;

	std = atoi(s);
	ft = ft_atoi(s);

	printf("input: \"%s\"\n", s);
	printf("atoi:     %d\n", std);
	printf("ft_atoi:  %d\n", ft);

	if (std != ft)
		printf("MISMATCH\n");
	else
		printf("OK");

	printf("\n----------------------------\n");
}

int	main(void)
{
	run_test("0");
	run_test("42");
	run_test("-42");
	run_test("+42");

	run_test("   42");
	run_test("\t\n 42");
	run_test("   -42");

	run_test("42abc");
	run_test("123test456");

	run_test("abc123");
	run_test("");
	run_test(" ");

	run_test("+");
	run_test("-");
	run_test("+-42");
	run_test("-+42");

	run_test("2147483647");
	run_test("-2147483648");

	run_test("00042");
	run_test("   +000123");

	run_test("999999999999999");
	run_test("-999999999999999");

	return (0);
}