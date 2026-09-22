#include <stdio.h>
#include "../../libft.h"

int	main(void)
{
	printf("'A' -> %d (expected 1)\n", ft_isalnum('A'));
	printf("'z' -> %d (expected 1)\n", ft_isalnum('z'));
	printf("'5' -> %d (expected 1)\n", ft_isalnum('5'));

	printf("'#' -> %d (expected 0)\n", ft_isalnum('#'));
	printf("' ' -> %d (expected 0)\n", ft_isalnum(' '));
	printf("'/' -> %d (expected 0)\n", ft_isalnum('/'));

	return (0);
}