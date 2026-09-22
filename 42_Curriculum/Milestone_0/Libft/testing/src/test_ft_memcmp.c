#include <stdio.h>
#include <string.h>
#include "../../libft.h"

static void	print_bytes(const unsigned char *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		printf("%02X ", s[i]);
		i++;
	}
	printf("\n");
}

static void	run_test(const void *s1, const void *s2, size_t n, const char *label)
{
	int	std;
	int	ft;

	std = memcmp(s1, s2, n);
	ft = ft_memcmp(s1, s2, n);

	printf("Test: %s\n", label);
	printf("n: %zu\n", n);

	printf("buffer1: ");
	print_bytes((const unsigned char *)s1, n);

	printf("buffer2: ");
	print_bytes((const unsigned char *)s2, n);

	printf("memcmp:    %d\n", std);
	printf("ft_memcmp: %d\n", ft);

	if ((std == 0 && ft != 0)
		|| (std < 0 && ft >= 0)
		|| (std > 0 && ft <= 0))
		printf("SIGN MISMATCH\n");

	printf("------------------------------\n");
}

int	main(void)
{
	unsigned char	a1[] = {'a','b','c','d','e'};
	unsigned char	a2[] = {'a','b','c','d','e'};
	unsigned char	b1[] = {'a','b','c','d','e'};
	unsigned char	b2[] = {'a','b','c','x','e'};
	unsigned char	c1[] = {0,1,2,3,4,5};
	unsigned char	c2[] = {0,1,2,3,4,6};
	unsigned char	d1[] = {255,128,42,0,10};
	unsigned char	d2[] = {255,128,41,0,10};

	run_test(a1, a2, 5, "identical buffers");
	run_test(b1, b2, 5, "difference in middle");
	run_test(b1, b2, 3, "difference outside range");
	run_test(b1, b2, 4, "difference at boundary");

	run_test(c1, c2, 6, "binary data difference");
	run_test(c1, c2, 5, "binary difference outside range");

	run_test(d1, d2, 5, "unsigned byte comparison");

	run_test("hello", "hello", 5, "identical strings");
	run_test("hello", "hellp", 5, "last char differs");
	run_test("hello", "hellp", 4, "difference outside n");

	run_test("abc", "abc", 0, "n = 0");

	return (0);
}