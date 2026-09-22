#include <stdio.h>
#include <string.h>
#include "../../libft.h"

static void	print_buffer(char *buf, size_t n)
{
	size_t	i = 0;

	while (i < n)
	{
		if (buf[i] == '\0')
			printf("\\0 ");
		else
			printf("%c ", buf[i]);
		i++;
	}
	printf("\n");
}

static void	run_test(const char *src, size_t size)
{
	char	dst_std[50];
	char	dst_ft[50];
	size_t	ret_std;
	size_t	ret_ft;

	memset(dst_std, 'X', sizeof(dst_std));
	memset(dst_ft, 'X', sizeof(dst_ft));

	ret_std = strlcpy(dst_std, src, size);
	ret_ft = ft_strlcpy(dst_ft, src, size);

	printf("SRC: \"%s\" | size: %zu\n", src, size);

	printf("Return values\n");
	printf("strlcpy:    %zu\n", ret_std);
	printf("ft_strlcpy: %zu\n", ret_ft);

	if (size > 0)
	{
		printf("\nDestination strings\n");
		printf("strlcpy:    \"%s\"\n", dst_std);
		printf("ft_strlcpy: \"%s\"\n", dst_ft);
	}
	else
		printf("\nDestination strings skipped because size == 0\n");

	printf("\nRaw buffers (first 50 bytes)\n");
	printf("strlcpy:    ");
	print_buffer(dst_std, 50);

	printf("ft_strlcpy: ");
	print_buffer(dst_ft, 50);

	if (ret_std != ret_ft)
		printf("RETURN VALUE MISMATCH\n");

	if (memcmp(dst_std, dst_ft, 50) != 0)
		printf("BUFFER CONTENT MISMATCH\n");

	printf("---------------------------------\n\n");
}

int	main(void)
{
	run_test("Hello", 10);
	run_test("Hello", 6);
	run_test("Hello", 5);
	run_test("Hello", 3);
	run_test("Hello", 1);
	run_test("Hello", 0);

	run_test("", 5);
	run_test("A", 2);

	run_test("This is a long string used for testing", 10);
	run_test("This is a long string used for testing", 50);

	return (0);
}