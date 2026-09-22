#include <stdio.h>
#include <string.h>
#include "../../libft.h"

static void	print_buf(unsigned char *buf, size_t n)
{
	size_t	i;

	i = 0;
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

	printf("===== BASIC COPY =====\n");

	ft_memmove(dst, src, 12);

	printf("expected: Hello World\n");
	printf("result:   %s\n\n", dst);
}

static void	test_overlap_forward(void)
{
	char	buf[20] = "123456789";

	printf("===== OVERLAP FORWARD =====\n");

	ft_memmove(buf + 2, buf, 5);

	printf("expected pattern: 121234589\n");
	printf("result:           %s\n\n", buf);
}

static void	test_overlap_backward(void)
{
	char	buf[20] = "123456789";

	printf("===== OVERLAP BACKWARD =====\n");

	ft_memmove(buf, buf + 2, 5);

	printf("expected pattern: 345676789\n");
	printf("result:           %s\n\n", buf);
}

static void	test_binary(void)
{
	unsigned char	src[8] = {0,1,2,3,4,5,6,7};
	unsigned char	dst[8];

	printf("===== BINARY COPY =====\n");

	ft_memmove(dst, src, 8);

	printf("expected:\n");
	print_buf(src, 8);

	printf("result:\n");
	print_buf(dst, 8);

	printf("\n");
}

static void	test_zero(void)
{
	char	src[] = "ABCDE";
	char	dst[10] = "XXXXXXXX";

	printf("===== ZERO LENGTH =====\n");

	ft_memmove(dst, src, 0);

	printf("expected: XXXXXXXX\n");
	printf("result:   %s\n\n", dst);
}

int	main(void)
{
	test_basic();
	test_overlap_forward();
	test_overlap_backward();
	test_binary();
	test_zero();
	return (0);
}