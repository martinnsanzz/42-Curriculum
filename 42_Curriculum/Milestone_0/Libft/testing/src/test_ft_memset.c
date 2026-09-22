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
	char str[20] = "HelloWorld";
	unsigned char	buf[10];
	int				i = 0;

	while (i < 10)
		buf[i++] = 0;

	printf("Test 1: fill with 'A'\n");
	ft_memset(buf, 'A', 10);
	print_buf(buf, 10);

	printf("Test 2: partial fill\n");
	ft_memset(buf, 'B', 5);
	print_buf(buf, 10);

	printf("Test 3: fill with 0\n");
	ft_memset(buf, 0, 10);
	print_buf(buf, 10);

	printf("Test 4: n = 0\n");
	ft_memset(buf, 'C', 0);
	print_buf(buf, 10);

	printf("Test 5: n = 0\n");
	ft_memset(str + 5, '*', 3);
	printf("%s\n", str);

	return (0);
}