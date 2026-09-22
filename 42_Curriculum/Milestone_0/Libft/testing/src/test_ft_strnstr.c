#include <stdio.h>
#include "../../libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

static int	g_pass = 0;
static int	g_fail = 0;

/* Pointer-based check (same offset into string) */
static void	check_ptr(const char *desc, const char *hay,
		char *got, char *expected)
{
	int	ok;

	ok = (got == NULL && expected == NULL)
		|| (got != NULL && expected != NULL
			&& (got - hay) == (expected - hay));
	if (ok)
	{
		printf("  [PASS] %s\n", desc);
		g_pass++;
	}
	else
	{
		printf("  [FAIL] %s\n", desc);
		printf("         expected offset: %td  str: %s\n",
			expected ? expected - hay : -1L,
			expected ? expected : "(null)");
		printf("         got      offset: %td  str: %s\n",
			got ? got - hay : -1L,
			got ? got : "(null)");
		g_fail++;
	}
}

/* ─── Section 1: empty needle ──────────────────────────────────────────── */
static void	test_empty_needle(void)
{
	const char	*h = "hello world";

	printf("\n[Section 1] Empty needle\n");
	check_ptr("empty needle returns haystack start",
		h, ft_strnstr(h, "", 10), (char *)h);
	check_ptr("empty needle, len=0 returns haystack start",
		h, ft_strnstr(h, "", 0), (char *)h);
	check_ptr("empty needle, empty haystack",
		"", ft_strnstr("", "", 0), (char *)"");
}

/* ─── Section 2: len = 0 ────────────────────────────────────────────────── */
static void	test_len_zero(void)
{
	const char	*h = "hello";

	printf("\n[Section 2] len = 0 (non-empty needle)\n");
	check_ptr("len=0, needle not empty -> NULL",
		h, ft_strnstr(h, "h", 0), NULL);
	check_ptr("len=0, any needle -> NULL",
		h, ft_strnstr(h, "hello", 0), NULL);
}

/* ─── Section 3: needle found (basic) ──────────────────────────────────── */
static void	test_found_basic(void)
{
	const char	*h = "hello world";

	printf("\n[Section 3] Needle found (basic cases)\n");
	check_ptr("needle at start",
		h, ft_strnstr(h, "hello", 11), (char *)h);
	check_ptr("needle in middle",
		h, ft_strnstr(h, "lo wo", 11), (char *)(h + 3));
	check_ptr("needle at end",
		h, ft_strnstr(h, "world", 11), (char *)(h + 6));
	check_ptr("single char needle",
		h, ft_strnstr(h, "w", 11), (char *)(h + 6));
	check_ptr("full string as needle",
		h, ft_strnstr(h, "hello world", 11), (char *)h);
}

/* ─── Section 4: needle NOT found ──────────────────────────────────────── */
static void	test_not_found(void)
{
	const char	*h = "hello world";

	printf("\n[Section 4] Needle not found\n");
	check_ptr("needle absent",
		h, ft_strnstr(h, "xyz", 11), NULL);
	check_ptr("needle longer than haystack",
		h, ft_strnstr(h, "hello world extra", 11), NULL);
	check_ptr("needle cut off by len",
		h, ft_strnstr(h, "world", 9), NULL);
	check_ptr("needle starts just past len",
		h, ft_strnstr(h, "world", 6), NULL);
	check_ptr("partial match at boundary -> NULL",
		h, ft_strnstr(h, "worldx", 11), NULL);
}

/* ─── Section 5: len boundary (needle fits exactly) ─────────────────────── */
static void	test_len_boundary(void)
{
	const char	*h = "abcdef";

	printf("\n[Section 5] len boundary (tight fits)\n");
	check_ptr("needle fits exactly within len",
		h, ft_strnstr(h, "abcdef", 6), (char *)h);
	check_ptr("len one less than required -> NULL",
		h, ft_strnstr(h, "abcdef", 5), NULL);
	check_ptr("needle at offset, len covers it exactly",
		h, ft_strnstr(h, "cdef", 6), (char *)(h + 2));
	check_ptr("needle at offset, len one short -> NULL",
		h, ft_strnstr(h, "cdef", 5), NULL);
}

/* ─── Section 6: repeated patterns ─────────────────────────────────────── */
static void	test_repeated_patterns(void)
{
	const char	*h = "aaaaab";
	const char	*h2 = "ababab";

	printf("\n[Section 6] Repeated / overlapping patterns\n");
	check_ptr("partial match then real match",
		h, ft_strnstr(h, "aaab", 6), (char *)(h + 2));
	check_ptr("overlapping pattern first occurrence",
		h2, ft_strnstr(h2, "abab", 6), (char *)h2);
	check_ptr("all same chars, needle = all",
		h, ft_strnstr(h, "aaaaab", 6), (char *)h);
	check_ptr("all same chars, needle longer -> NULL",
		h, ft_strnstr(h, "aaaaaab", 6), NULL);
}

/* ─── Section 7: haystack == needle ─────────────────────────────────────── */
static void	test_equal_strings(void)
{
	const char	*h = "exact";

	printf("\n[Section 7] haystack == needle\n");
	check_ptr("exact match",
		h, ft_strnstr(h, "exact", 5), (char *)h);
	check_ptr("exact match, len just enough",
		h, ft_strnstr(h, "exact", 5), (char *)h);
	check_ptr("exact match, len too small -> NULL",
		h, ft_strnstr(h, "exact", 4), NULL);
}

/* ─── Section 8: large len (bigger than string) ─────────────────────────── */
static void	test_large_len(void)
{
	const char	*h = "tiny";

	printf("\n[Section 8] len >> strlen(haystack)\n");
	check_ptr("len much larger, needle found",
		h, ft_strnstr(h, "tiny", 9999), (char *)h);
	check_ptr("len much larger, needle absent",
		h, ft_strnstr(h, "nope", 9999), NULL);
}

/* ─── Section 9: single-character edge cases ────────────────────────────── */
static void	test_single_chars(void)
{
	const char	*h = "abcde";

	printf("\n[Section 9] Single character\n");
	check_ptr("first char",
		h, ft_strnstr(h, "a", 5), (char *)h);
	check_ptr("last char, len covers it",
		h, ft_strnstr(h, "e", 5), (char *)(h + 4));
	check_ptr("last char, len misses by one -> NULL",
		h, ft_strnstr(h, "e", 4), NULL);
	check_ptr("char not in string",
		h, ft_strnstr(h, "z", 5), NULL);
}

/* ─── Section 10: special / tricky strings ──────────────────────────────── */
static void	test_special(void)
{
	const char	*h = "foo\0bar";
	const char	*h2 = "   spaces   ";

	printf("\n[Section 10] Special strings\n");
	/* haystack stops at NUL — needle "bar" must NOT be found */
	check_ptr("needle after embedded NUL -> NULL",
		h, ft_strnstr(h, "bar", 7), NULL);
	check_ptr("spaces in haystack",
		h2, ft_strnstr(h2, "spaces", 12), (char *)(h2 + 3));
	check_ptr("needle is a space",
		h2, ft_strnstr(h2, " ", 12), (char *)h2);
}

/* ─── Summary ────────────────────────────────────────────────────────────── */
int	main(void)
{
	printf("=== ft_strnstr tester ===");
	test_empty_needle();
	test_len_zero();
	test_found_basic();
	test_not_found();
	test_len_boundary();
	test_repeated_patterns();
	test_equal_strings();
	test_large_len();
	test_single_chars();
	test_special();
	printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
	return (g_fail > 0 ? 1 : 0);
}