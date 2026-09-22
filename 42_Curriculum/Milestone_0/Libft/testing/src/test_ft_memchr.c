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

static void	run_test(const void *s, int c, size_t n, const char *label)
{
	void				*std;
	void				*ft;
	const unsigned char	*bytes;

	bytes = (const unsigned char *)s;
	std = memchr(s, c, n);
	ft = ft_memchr(s, c, n);

	printf("Test: %s\n", label);
	printf("c: %d", c);
	if (c >= 32 && c <= 126)
		printf(" ('%c')", c);
	printf("\n");
	printf("n: %zu\n", n);
	printf("buffer: ");
	print_bytes(bytes, n);

	printf("memchr:    %p", std);
	if (std != NULL)
		printf(" (offset %ld)", (long)((unsigned char *)std - bytes));
	printf("\n");

	printf("ft_memchr: %p", ft);
	if (ft != NULL)
		printf(" (offset %ld)", (long)((unsigned char *)ft - bytes));
	printf("\n");

	if (std != ft)
		printf("POINTER MISMATCH\n");
	printf("-----------------------------\n");
}

int	main(void)
{
	unsigned char	buf1[] = {'a', 'b', 'c', 'd', 'e', '\0'};
	unsigned char	buf2[] = {0, 1, 2, 3, 4, 5, 6, 7};
	unsigned char	buf3[] = {'x', 'x', 'x', 'x', 'x'};
	unsigned char	buf4[] = {'a', '\0', 'b', '\0', 'c', '\0'};
	unsigned char	buf5[] = {255, 128, 42, 0, 127};

	run_test(buf1, 'a', 6, "find first byte");
	run_test(buf1, 'c', 6, "find middle byte");
	run_test(buf1, 'e', 6, "find last visible char");
	run_test(buf1, '\0', 6, "find null byte");
	run_test(buf1, 'z', 6, "byte not found");

	run_test(buf2, 0, 8, "find zero in binary buffer");
	run_test(buf2, 4, 8, "find number in binary buffer");
	run_test(buf2, 7, 8, "find last byte in binary buffer");
	run_test(buf2, 8, 8, "not found in binary buffer");

	run_test(buf3, 'x', 5, "repeated bytes");
	run_test(buf3, 'x', 0, "n = 0");
	run_test(buf3, 'y', 5, "repeated bytes not found");

	run_test(buf4, '\0', 6, "null byte inside buffer");
	run_test(buf4, 'b', 6, "search after null byte");

	run_test(buf5, 255, 5, "unsigned char 255");
	run_test(buf5, 128, 5, "unsigned char 128");
	run_test(buf5, 127, 5, "unsigned char 127");

	run_test("hello world", 'o', 11, "string literal");
	run_test("hello world", 'o', 5, "cut search range");
	run_test("hello world", 'h', 1, "single-byte range");
	run_test("hello world", 'e', 1, "single-byte miss");

	return (0);
}