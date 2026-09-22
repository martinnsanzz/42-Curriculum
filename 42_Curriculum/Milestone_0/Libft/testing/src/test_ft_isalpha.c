#include <stdio.h>
#include "../../libft.h"

int	main(void)
{
	printf("A -> %d (expected 1)\n", ft_isalpha('A'));
	printf("z -> %d (expected 1)\n", ft_isalpha('z'));
	printf("M -> %d (expected 1)\n", ft_isalpha('M'));

	printf("0 -> %d (expected 0)\n", ft_isalpha('0'));
	printf("# -> %d (expected 0)\n", ft_isalpha('#'));
	printf("space -> %d (expected 0)\n", ft_isalpha(' '));

	printf("@ boundary -> %d (expected 0)\n", ft_isalpha('@'));
	printf("[ boundary -> %d (expected 0)\n", ft_isalpha('['));
	printf("` boundary -> %d (expected 0)\n", ft_isalpha('`'));
	printf("{ boundary -> %d (expected 0)\n", ft_isalpha('{'));

	return (0);
}