#include <stdio.h>
#include <ctype.h>
#include "../../libft.h"

static void	run_test(int c)
{
	int	std;
	int	ft;

	std = toupper(c);
	ft = ft_toupper(c);

	printf("Input: %d ('%c')\n", c, (c >= 32 && c <= 126) ? c : '.');

	printf("toupper:    %d ('%c')\n",
		std, (std >= 32 && std <= 126) ? std : '.');

	printf("ft_toupper: %d ('%c')\n",
		ft, (ft >= 32 && ft <= 126) ? ft : '.');

	if (std != ft)
		printf("MISMATCH\n");

	printf("--------------------------\n");
}

int	main(void)
{
	/* lowercase letters */
	run_test('a');
	run_test('m');
	run_test('z');

	/* uppercase letters */
	run_test('A');
	run_test('M');
	run_test('Z');

	/* digits */
	run_test('0');
	run_test('5');
	run_test('9');

	/* symbols */
	run_test('#');
	run_test('!');
	run_test('{');

	/* ASCII boundaries */
	run_test(0);
	run_test(31);
	run_test(32);
	run_test(126);
	run_test(127);

	/* extended values */
	run_test(200);
	run_test(255);

	return (0);
}