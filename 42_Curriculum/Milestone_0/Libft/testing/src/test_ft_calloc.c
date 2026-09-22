#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../../libft.h"

static void	print_bytes(unsigned char *p, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		printf("%02X ", p[i]);
		i++;
	}
	printf("\n");
}

static void	run_normal_test(size_t count, size_t size)
{
	void	*std;
	void	*ft;
	size_t	total;

	total = count * size;
	std = calloc(count, size);
	ft = ft_calloc(count, size);

	printf("NORMAL TEST\n");
	printf("count: %zu | size: %zu | total: %zu\n", count, size, total);

	printf("calloc:    %p\n", std);
	printf("ft_calloc: %p\n", ft);

	if (std == NULL || ft == NULL)
	{
		printf("One allocation returned NULL\n");
		printf("-----------------------------\n");
		free(std);
		free(ft);
		return ;
	}
	if (memcmp(std, ft, total) != 0)
		printf("MEMORY CONTENT MISMATCH\n");
	else
		printf("Memory content identical\n");
	if (total > 0)
	{
		printf("First bytes calloc:    ");
		print_bytes((unsigned char *)std, total > 16 ? 16 : total);
		printf("First bytes ft_calloc: ");
		print_bytes((unsigned char *)ft, total > 16 ? 16 : total);
	}
	free(std);
	free(ft);
	printf("-----------------------------\n");
}

static void	run_zero_test(size_t count, size_t size)
{
	void	*std;
	void	*ft;

	std = calloc(count, size);
	ft = ft_calloc(count, size);

	printf("ZERO-SIZE TEST\n");
	printf("count: %zu | size: %zu\n", count, size);
	printf("calloc:    %p\n", std);
	printf("ft_calloc: %p\n", ft);
	printf("Note: for zero-sized allocation, NULL and non-NULL are both allowed,\n");
	printf("but the pointer must be safely freeable.\n");
	free(std);
	free(ft);
	printf("-----------------------------\n");
}

static void	run_overflow_test(size_t count, size_t size)
{
	void	*std;
	void	*ft;

	std = calloc(count, size);
	ft = ft_calloc(count, size);

	printf("OVERFLOW TEST\n");
	printf("count: %zu | size: %zu\n", count, size);
	printf("calloc:    %p\n", std);
	printf("ft_calloc: %p\n", ft);

	if (std == NULL && ft == NULL)
		printf("OK: both returned NULL\n");
	else
		printf("POTENTIAL OVERFLOW BUG\n");
	free(std);
	free(ft);
	printf("-----------------------------\n");
}

int	main(void)
{
	run_normal_test(1, 1);
	run_normal_test(5, 1);
	run_normal_test(10, sizeof(int));
	run_normal_test(8, sizeof(long));
	run_normal_test(100, sizeof(char));

	run_zero_test(0, 10);
	run_zero_test(10, 0);
	run_zero_test(0, 0);

	run_overflow_test(SIZE_MAX, 2);
	run_overflow_test(SIZE_MAX / 2 + 1, 2);
	run_overflow_test(SIZE_MAX / 4 + 1, 8);
	run_overflow_test(SIZE_MAX, SIZE_MAX);
	run_overflow_test((size_t)-1, 16);
	return (0);
}