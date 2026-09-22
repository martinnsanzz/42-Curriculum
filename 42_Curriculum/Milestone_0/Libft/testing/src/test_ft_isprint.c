#include <stdio.h>
#include "../../libft.h"

int	main(void)
{
	printf("' ' -> %d (expected 1)\n", ft_isprint(' '));
	printf("'~' -> %d (expected 1)\n", ft_isprint('~'));
	printf("'A' -> %d (expected 1)\n", ft_isprint('A'));

	printf("31 -> %d (expected 0)\n", ft_isprint(31));
	printf("127 -> %d (expected 0)\n", ft_isprint(127));
	printf("\\n -> %d (expected 0)\n", ft_isprint('\n'));

	return (0);
}