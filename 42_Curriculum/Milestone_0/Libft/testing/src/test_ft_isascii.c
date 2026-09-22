#include <stdio.h>
#include "../../libft.h"

int	main(void)
{
	printf("0 -> %d (expected 1)\n", ft_isascii(0));
	printf("65 -> %d (expected 1)\n", ft_isascii(65));
	printf("256 -> %d (expected 0)\n", ft_isascii(256));

	printf("-1 -> %d (expected 0)\n", ft_isascii(-1));
	printf("128 -> %d (expected 0)\n", ft_isascii(128));
	printf("200 -> %d (expected 0)\n", ft_isascii(200));

	return (0);
}