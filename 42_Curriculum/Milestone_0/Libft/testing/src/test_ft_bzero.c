#include <stdio.h>
#include "../../libft.h"

static void	print_buf(unsigned char *buf, int n)
{
	int	i = 0;

	while (i < n)
	{
		printf("%02X ", buf[i]);
		i++;
	}
	printf("\n");
}

int	main(void)
{
	unsigned char	buf[10];
	int				i = 0;

	while (i < 10)
		buf[i++] = 'X';

	printf("Initial buffer\n");
	print_buf(buf, 10);

	printf("Test 1: zero first 5 bytes\n");
	ft_bzero(buf, 5);
	print_buf(buf, 10);

	printf("Test 2: zero entire buffer\n");
	ft_bzero(buf, 10);
	print_buf(buf, 10);

	printf("Test 3: n = 0\n");
	ft_bzero(buf, 0);
	print_buf(buf, 10);

	return (0);
}