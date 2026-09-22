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

static void	run_test(const char *dst_init, const char *src, size_t size)
{
	char	dst_std[100];
	char	dst_ft[100];
	size_t	ret_std;
	size_t	ret_ft;

	memset(dst_std, 'X', sizeof(dst_std));
	memset(dst_ft, 'X', sizeof(dst_ft));

	strcpy(dst_std, dst_init);
	strcpy(dst_ft, dst_init);

	ret_std = strlcat(dst_std, src, size);
	ret_ft = ft_strlcat(dst_ft, src, size);

	printf("DST: \"%s\" | SRC: \"%s\" | size: %zu\n", dst_init, src, size);

	printf("Return values\n");
	printf("strlcat:    %zu\n", ret_std);
	printf("ft_strlcat: %zu\n", ret_ft);

	printf("\nResult strings\n");
	printf("strlcat:    \"%s\"\n", dst_std);
	printf("ft_strlcat: \"%s\"\n", dst_ft);

	printf("\nRaw buffer (first 30 bytes)\n");
	printf("strlcat:    ");
	print_buffer(dst_std, 30);

	printf("ft_strlcat: ");
	print_buffer(dst_ft, 30);

	if (ret_std != ret_ft)
		printf("RETURN VALUE MISMATCH\n");

	if (memcmp(dst_std, dst_ft, 30) != 0)
		printf("BUFFER CONTENT MISMATCH\n");

	printf("----------------------------------\n\n");
}

int	main(void)
{
	run_test("Hello", "World", 20);   // full append
	run_test("Hello", "World", 10);   // truncated append
	run_test("Hello", "World", 8);    // heavy truncation
	run_test("Hello", "World", 6);    // almost no space
	run_test("Hello", "World", 5);    // no append
	run_test("Hello", "World", 3);    // size < dst length
	run_test("", "World", 10);        // empty dst
	run_test("A", "BCDEF", 4);        // partial append
	run_test("Test", "", 10);         // empty src
	run_test("LongDestination", "ABC", 5); // dst longer than size

	return (0);
}