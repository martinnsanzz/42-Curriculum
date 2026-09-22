#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

static void	run_test(int n)
{
	char	*expected;
	char	*result;

	expected = NULL;
	result = ft_itoa(n);

	/* Use stdlib itoa equivalent via sprintf */
	expected = malloc(32);
	if (!expected)
		return ;
	sprintf(expected, "%d", n);

	printf("Input:    %d\n", n);
	printf("Expected: \"%s\"\n", expected);
	printf("ft_itoa:  \"%s\"\n", result ? result : "(null)");

	if (!result)
		printf("FAIL: returned NULL\n");
	else if (strcmp(expected, result) != 0)
		printf("FAIL: mismatch\n");
	else
		printf("OK\n");

	free(expected);
	free(result);
	printf("----------------------------------\n\n");
}

int	main(void)
{
	run_test(-42);           // negative
	run_test(-1);            // negative single digit
	run_test(-2147483648);   // INT_MIN (tricky: -INT_MIN overflows)
	run_test(-100);          // negative round
	run_test(-0);             // zero
	run_test(1);             // single digit
	run_test(42);            // positive
	run_test(2147483647);    // INT_MAX
	run_test(100);           // round number
	run_test(1000000000);    // large positive

	return (0);
}