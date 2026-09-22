#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../libft.h"

static void	run_test(const char *s)
{
	char	*std;
	char	*ft;

	std = strdup(s);
	ft = ft_strdup(s);

	printf("input: \"%s\"\n", s);

	printf("strdup:    %p -> \"%s\"\n", (void *)std, std);
	printf("ft_strdup: %p -> \"%s\"\n", (void *)ft, ft);

	if (strcmp(std, ft) != 0)
		printf("CONTENT MISMATCH\n");
	else
		printf("Content identical\n");

	if (ft == s)
		printf("ERROR: ft_strdup returned original pointer\n");

	if (std == ft)
		printf("ERROR: ft_strdup returned same pointer as strdup\n");

	printf("-----------------------------\n");

	free(std);
	free(ft);
}

int	main(void)
{
	run_test("hello");
	run_test("");
	run_test("a");
	run_test("42 school");
	run_test("long string used for testing ft_strdup implementation");

	run_test("aaaaaa");
	run_test("string with spaces");
	run_test("1234567890");

	return (0);
}