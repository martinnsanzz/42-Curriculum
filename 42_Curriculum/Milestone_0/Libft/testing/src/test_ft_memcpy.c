#include <stdio.h>
#include <string.h>
#include "../../libft.h"

static void	print_buf(unsigned char *buf, size_t n)
{
	size_t	i = 0;

	while (i < n)
	{
		printf("%02X ", buf[i]);
		i++;
	}
	printf("\n");
}

static void	test_basic(void)
{
	char	src[] = "Hello World";
	char	dst[20];

	printf("===== BASIC STRING COPY =====\n");

	ft_memcpy(dst, src, 12);

	printf("expected: Hello World\n");
	printf("result:   %s\n\n", dst);
}

static void	test_partial(void)
{
	char	src[] = "ABCDEFGHIJ";
	char	dst[20];

	printf("===== PARTIAL COPY =====\n");

	ft_memcpy(dst, src, 5);
	dst[5] = '\0';

	printf("expected: ABCDE\n");
	printf("result:   %s\n\n", dst);
}

static void	test_binary(void)
{
	unsigned char	src[8] = {0,1,2,3,4,5,6,7};
	unsigned char	dst[8];

	printf("===== BINARY COPY =====\n");

	ft_memcpy(dst, src, 8);

	printf("expected:\n");
	print_buf(src, 8);

	printf("result:\n");
	print_buf(dst, 8);

	printf("\n");
}

static void	test_offset(void)
{
	char	buf[20] = "123456789";

	printf("===== POINTER OFFSET COPY =====\n");

	ft_memcpy(buf + 4, "ABC", 3);

	printf("expected: 1234ABC89\n");
	printf("result:   %s\n\n", buf);
}

static void	test_zero(void)
{
	char	src[] = "HELLO";
	char	dst[20] = "XXXXXXXX";

	printf("===== ZERO LENGTH COPY =====\n");

	ft_memcpy(dst, src, 0);

	printf("expected: XXXXXXXX\n");
	printf("result:   %s\n\n", dst);
}

int	main(void)
{
	test_basic();
	test_partial();
	test_binary();
	test_offset();
	test_zero();
	return (0);
}