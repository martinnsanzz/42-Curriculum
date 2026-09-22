#include <stdio.h>
#include "../../libft.h"

int	main(void)
{
	printf("Test 1: \"Hello\"\n");
	printf("expected: 5\n");
	printf("result:   %zu\n\n", ft_strlen("Hello"));

	printf("Test 2: empty string\n");
	printf("expected: 0\n");
	printf("result:   %zu\n\n", ft_strlen(""));

	printf("Test 3: single char\n");
	printf("expected: 1\n");
	printf("result:   %zu\n\n", ft_strlen("A"));

	printf("Test 4: spaces\n");
	printf("expected: 4\n");
	printf("result:   %zu\n\n", ft_strlen("    "));

	printf("Test 5: long string\n");
	printf("expected: 26\n");
	printf("result:   %zu\n\n", ft_strlen("abcdefghijklmnopqrstuvwxyz"));

	return (0);
}