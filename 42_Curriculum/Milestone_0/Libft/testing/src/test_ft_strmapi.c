#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

/* ==================== HELPER FUNCTIONS ==================== */

static char	to_upper(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static char	to_lower(unsigned int i, char c)
{
	(void)i;
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static char	add_index(unsigned int i, char c)
{
	return (c + (char)i);
}

static char	replace_with_index(unsigned int i, char c)
{
	(void)c;
	return ('0' + (char)(i % 10));
}

static char	identity(unsigned int i, char c)
{
	(void)i;
	return (c);
}

static char	replace_all_a(unsigned int i, char c)
{
	(void)i;
	(void)c;
	return ('a');
}

static char	zero_all(unsigned int i, char c)
{
	(void)i;
	(void)c;
	return (0);
}

/* ==================== TEST RUNNER ==================== */

static int	g_pass = 0;
static int	g_fail = 0;

static void	check(const char *desc, char *result, const char *expected)
{
	printf("  TEST: %s\n", desc);
	if (result == NULL && expected == NULL)
	{
		printf("    Input:    NULL\n");
		printf("    Expected: NULL\n");
		printf("    Got:      NULL\n");
		printf("    [PASS]\n\n");
		g_pass++;
		return ;
	}
	if (result == NULL)
	{
		printf("    Expected: \"%s\"\n", expected);
		printf("    Got:      NULL\n");
		printf("    [FAIL] -- returned NULL unexpectedly\n\n");
		g_fail++;
		return ;
	}
	printf("    Expected: \"%s\"\n", expected);
	printf("    Got:      \"%s\"\n", result);
	if (strcmp(result, expected) == 0)
	{
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL]\n\n");
		g_fail++;
	}
	free(result);
}

static void	check_null(const char *desc, char *result)
{
	printf("  TEST: %s\n", desc);
	printf("    Expected: NULL\n");
	if (result == NULL)
	{
		printf("    Got:      NULL\n");
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    Got:      \"%s\"\n", result);
		printf("    [FAIL] -- should have returned NULL\n\n");
		g_fail++;
		free(result);
	}
}

/* ==================== TESTS ==================== */

static void	test_normal_cases(void)
{
	char	*result;

	printf("=== NORMAL CASES ===\n\n");

	result = ft_strmapi("hello", to_upper);
	check("to_upper: \"hello\" -> \"HELLO\"", result, "HELLO");

	result = ft_strmapi("WORLD", to_lower);
	check("to_lower: \"WORLD\" -> \"world\"", result, "world");

	result = ft_strmapi("hello", identity);
	check("identity: \"hello\" -> \"hello\"", result, "hello");

	result = ft_strmapi("Hello World", to_upper);
	check("to_upper: mixed -> \"HELLO WORLD\"", result, "HELLO WORLD");

	result = ft_strmapi("abc", replace_with_index);
	check("index_as_char: \"abc\" -> \"012\"", result, "012");
}

static void	test_edge_cases(void)
{
	char	*result;
	char	buf[12];
	int		i;

	printf("=== EDGE CASES ===\n\n");

	result = ft_strmapi("", identity);
	check("empty string + identity -> \"\"", result, "");

	result = ft_strmapi("a", to_upper);
	check("single char: \"a\" -> \"A\"", result, "A");

	result = ft_strmapi("Z", to_lower);
	check("single char: \"Z\" -> \"z\"", result, "z");

	result = ft_strmapi("1234567890", replace_all_a);
	check("replace_all_a: \"1234567890\" -> \"aaaaaaaaaa\"", result, "aaaaaaaaaa");

	i = 0;
	while (i < 11)
	{
		buf[i] = 'a' + i;
		i++;
	}
	buf[11] = '\0';
	result = ft_strmapi(buf, to_upper);
	check("11 chars to_upper: \"abcdefghijk\" -> \"ABCDEFGHIJK\"", result, "ABCDEFGHIJK");
}

static void	test_index_passed_correctly(void)
{
	char	*result;
	char	expected[6];
	int		i;

	printf("=== INDEX PASSING ===\n\n");

	result = ft_strmapi("aaaaa", add_index);
	i = 0;
	while (i < 5)
	{
		expected[i] = 'a' + i;
		i++;
	}
	expected[5] = '\0';
	check("add_index: \"aaaaa\" -> \"abcde\"", result, expected);

	result = ft_strmapi("0123456789", replace_with_index);
	check("index as digit: \"0123456789\" -> \"0123456789\"", result, "0123456789");
}

static void	test_null_input(void)
{
	char	*result;

	printf("=== NULL INPUT ===\n\n");

	result = ft_strmapi(NULL, identity);
	check_null("NULL string + valid f -> NULL", result);
}

static void	test_new_string_allocated(void)
{
	char	*input;
	char	*result;

	printf("=== ALLOCATION / INDEPENDENCE ===\n\n");

	input = strdup("hello");
	if (!input)
		return ;
	result = ft_strmapi(input, to_upper);
	printf("  TEST: result is a new allocation (not same pointer)\n");
	if (result == NULL)
	{
		printf("    [FAIL] -- returned NULL\n\n");
		g_fail++;
		free(input);
		return ;
	}
	if (result != input)
	{
		printf("    Result ptr != input ptr: [PASS]\n");
		g_pass++;
	}
	else
	{
		printf("    Result ptr == input ptr: [FAIL] -- same pointer returned\n");
		g_fail++;
	}
	printf("    Original unchanged: \"%s\"\n", input);
	if (strcmp(input, "hello") == 0)
	{
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL] -- original was modified\n\n");
		g_fail++;
	}
	free(input);
	free(result);
}

static void	test_nul_char_in_result(void)
{
	char	*result;

	printf("=== NUL TERMINATION ===\n\n");

	result = ft_strmapi("abc", zero_all);
	printf("  TEST: f returns 0 for all chars\n");
	if (result == NULL)
	{
		printf("    Got:      NULL\n");
		printf("    [FAIL]\n\n");
		g_fail++;
		return ;
	}
	printf("    result[0] == 0: %s\n", result[0] == 0 ? "YES" : "NO");
	if (result[0] == 0)
	{
		printf("    [PASS] -- NUL char stored correctly\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL]\n\n");
		g_fail++;
	}
	free(result);
}

/* ==================== MAIN ==================== */

int	main(void)
{
	printf("\n========================================\n");
	printf("         TESTER: ft_strmapi\n");
	printf("========================================\n\n");

	test_normal_cases();
	test_edge_cases();
	test_index_passed_correctly();
	test_null_input();
	test_new_string_allocated();
	test_nul_char_in_result();

	printf("========================================\n");
	printf("  RESULTS: %d passed | %d failed\n", g_pass, g_fail);
	printf("========================================\n\n");
	return (g_fail > 0 ? 1 : 0);
}