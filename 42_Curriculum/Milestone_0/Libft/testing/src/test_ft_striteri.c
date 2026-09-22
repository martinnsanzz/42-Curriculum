#include <stdio.h>
#include <string.h>
#include "../../libft.h"

/* ==================== HELPER FUNCTIONS ==================== */

static void	to_upper(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'a' && *c <= 'z')
		*c -= 32;
}

static void	to_lower(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'A' && *c <= 'Z')
		*c += 32;
}

static void	add_index(unsigned int i, char *c)
{
	*c += (char)i;
}

static void	replace_with_index(unsigned int i, char *c)
{
	*c = '0' + (char)(i % 10);
}

static void	replace_all_x(unsigned int i, char *c)
{
	(void)i;
	*c = 'x';
}

static void	identity(unsigned int i, char *c)
{
	(void)i;
	(void)c;
}

static unsigned int	g_last_index = 0;
static char			g_last_char = 0;

static void	record_last(unsigned int i, char *c)
{
	g_last_index = i;
	g_last_char = *c;
}

/* ==================== TEST RUNNER ==================== */

static int	g_pass = 0;
static int	g_fail = 0;

static void	check(const char *desc, const char *result, const char *expected)
{
	printf("  TEST: %s\n", desc);
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
}

/* ==================== TESTS ==================== */

static void	test_normal_cases(void)
{
	char	s1[] = "hello";
	char	s2[] = "WORLD";
	char	s3[] = "Hello World";
	char	s4[] = "hello";

	printf("=== NORMAL CASES ===\n\n");

	ft_striteri(s1, to_upper);
	check("to_upper: \"hello\" -> \"HELLO\"", s1, "HELLO");

	ft_striteri(s2, to_lower);
	check("to_lower: \"WORLD\" -> \"world\"", s2, "world");

	ft_striteri(s3, to_upper);
	check("to_upper: \"Hello World\" -> \"HELLO WORLD\"", s3, "HELLO WORLD");

	ft_striteri(s4, identity);
	check("identity: \"hello\" -> \"hello\"", s4, "hello");
}

static void	test_in_place_modification(void)
{
	char	s1[] = "abcde";
	char	s2[] = "12345";

	printf("=== IN-PLACE MODIFICATION ===\n\n");

	ft_striteri(s1, replace_all_x);
	check("replace_all_x: \"abcde\" -> \"xxxxx\"", s1, "xxxxx");

	ft_striteri(s2, replace_all_x);
	check("replace_all_x: \"12345\" -> \"xxxxx\"", s2, "xxxxx");
}

static void	test_index_passing(void)
{
	char	s1[] = "aaaaa";
	char	s2[] = "0000000000";
	char	expected2[11];
	int		i;

	printf("=== INDEX PASSING ===\n\n");

	ft_striteri(s1, add_index);
	check("add_index: \"aaaaa\" -> \"abcde\"", s1, "abcde");

	ft_striteri(s2, replace_with_index);
	i = 0;
	while (i < 10)
	{
		expected2[i] = '0' + (i % 10);
		i++;
	}
	expected2[10] = '\0';
	check("replace_with_index: \"0000000000\" -> \"0123456789\"", s2, expected2);
}

static void	test_index_starts_at_zero(void)
{
	char	s[] = "a";

	printf("=== INDEX STARTS AT ZERO ===\n\n");

	g_last_index = 99;
	g_last_char = 0;
	ft_striteri(s, record_last);
	printf("  TEST: single char string, index passed to f == 0\n");
	printf("    Expected index: 0\n");
	printf("    Got index:      %u\n", g_last_index);
	if (g_last_index == 0)
	{
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL]\n\n");
		g_fail++;
	}
}

static void	test_last_index_correct(void)
{
	char	s[] = "abcde";

	printf("=== LAST INDEX CORRECT ===\n\n");

	g_last_index = 0;
	g_last_char = 0;
	ft_striteri(s, record_last);
	printf("  TEST: \"abcde\" last index == 4, last char == 'e'\n");
	printf("    Expected index: 4 | Got: %u\n", g_last_index);
	printf("    Expected char:  'e' | Got: '%c'\n", g_last_char);
	if (g_last_index == 4 && g_last_char == 'e')
	{
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL]\n\n");
		g_fail++;
	}
}

static void	test_edge_cases(void)
{
	char	s1[] = "a";
	char	s2[] = "Z";

	printf("=== EDGE CASES ===\n\n");

	ft_striteri(s1, to_upper);
	check("single char: \"a\" -> \"A\"", s1, "A");

	ft_striteri(s2, to_lower);
	check("single char: \"Z\" -> \"z\"", s2, "z");
}

static void	test_empty_string(void)
{
	char	s[] = "";

	printf("=== EMPTY STRING ===\n\n");

	g_last_index = 99;
	ft_striteri(s, record_last);
	printf("  TEST: empty string, f must never be called\n");
	printf("    g_last_index should remain 99: %s\n",
		g_last_index == 99 ? "[PASS]" : "[FAIL]");
	if (g_last_index == 99)
		g_pass++;
	else
		g_fail++;
	printf("\n");

	ft_striteri(s, identity);
	check("empty string + identity -> \"\"", s, "");
}

static void	test_null_input(void)
{
	printf("=== NULL INPUT ===\n\n");
	printf("  TEST: ft_striteri(NULL, identity)\n");
	printf("    Expected: no crash (graceful NULL guard)\n");
	ft_striteri(NULL, identity);
	printf("    [PASS] -- no crash\n\n");
	g_pass++;
}

static void	test_pointer_passed_to_f(void)
{
	char	s[] = "abc";
	char	*addr_received = NULL;

	printf("=== POINTER PASSED TO F ===\n\n");

	(void)addr_received;

	printf("  TEST: f receives address of each char (in-place edit)\n");
	ft_striteri(s, to_upper);
	printf("    After to_upper on \"abc\": \"%s\"\n", s);
	if (strcmp(s, "ABC") == 0)
	{
		printf("    [PASS] -- chars modified via pointer\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL] -- chars not modified via pointer\n\n");
		g_fail++;
	}
}

/* ==================== MAIN ==================== */

int	main(void)
{
	printf("\n========================================\n");
	printf("        TESTER: ft_striteri\n");
	printf("========================================\n\n");

	test_normal_cases();
	test_in_place_modification();
	test_index_passing();
	test_index_starts_at_zero();
	test_last_index_correct();
	test_edge_cases();
	test_empty_string();
	test_null_input();
	test_pointer_passed_to_f();

	printf("========================================\n");
	printf("  RESULTS: %d passed | %d failed\n", g_pass, g_fail);
	printf("========================================\n\n");
	return (g_fail > 0 ? 1 : 0);
}