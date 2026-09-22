#include <stdio.h>
#include "../../libft.h"

int	main(void)
{
	printf("0 -> %d (expected 1)\n", ft_isdigit('0'));
	printf("5 -> %d (expected 1)\n", ft_isdigit('5'));
	printf("9 -> %d (expected 1)\n", ft_isdigit('9'));

	printf("a -> %d (expected 0)\n", ft_isdigit('a'));
	printf("/ -> %d (expected 0)\n", ft_isdigit('/'));
	printf(": -> %d (expected 0)\n", ft_isdigit(':'));

	return (0);
}