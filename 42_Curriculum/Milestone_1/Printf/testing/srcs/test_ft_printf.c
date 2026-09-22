#include "../../ft_printf.h"
#include <stdio.h>
#include <limits.h>

void	test_ft_printf_c(void);
void	test_ft_printf_s(void);
void	test_ft_printf_d_i(void);
void	test_ft_printf_percent(void);
void	test_ft_printf_u(void);
void	test_ft_printf_hex(void);
void	test_ft_printf_p(void);

int	main(void)
{
	//test_ft_printf_d_i();
	//test_ft_printf_s();
	//test_ft_printf_c();
	//test_ft_printf_percent();
	//test_ft_printf_u();
	//test_ft_printf_hex();
	//test_ft_printf_p();
	//printf("%p\n", &a);
	//int a = printf("%u\n", -1);
	//int a2 = ft_printf("%u\n", -1);
	//printf("\n%d, %d\n", a, a2);
	return (0);
}

void	test_ft_printf_p(void)
{
	int		r1;
	int		r2;
	int		pass;
	int		fail;
	int		a;
	int		b;
	int		*ptr;
	void	*null_ptr;

	pass = 0;
	fail = 0;
	a = 42;
	b = 0;
	ptr = &a;
	null_ptr = NULL;
	printf("\n=== %%p TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("stack var",          "%p", (void *)&a)
	RUN("stack var 2",        "%p", (void *)&b)
	RUN("ptr to ptr",         "%p", (void *)&ptr)
	RUN("null ptr",           "%p", null_ptr)
	RUN("two %%p",            "%p%p", (void *)&a, (void *)&b)
	RUN("surrounded",         "[%p]", (void *)&a)
	RUN("mixed %%p %%d",      "%p %d", (void *)&a, 42)
	RUN("mixed %%p %%s",      "%p %s", (void *)&a, "hi")
	RUN("literal 0x0",        "%p", (void *)0)
	RUN("literal 0x1",        "%p", (void *)1)
	RUN("literal addr",       "%p", (void *)0xdeadbeef)
	RUN("UINTPTR_MAX",        "%p", (void *)UINTPTR_MAX)

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}

void	test_ft_printf_hex(void)
{
	int				r1;
	int				r2;
	int				pass;
	int				fail;

	pass = 0;
	fail = 0;
	printf("\n=== %%x / %%X TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("zero %%x",           "%x", 0)
	RUN("zero %%X",           "%X", 0)
	RUN("basic %%x",          "%x", 42)
	RUN("basic %%X",          "%X", 42)
	RUN("255 %%x",            "%x", 255)
	RUN("255 %%X",            "%X", 255)
	RUN("16 %%x",             "%x", 16)
	RUN("16 %%X",             "%X", 16)
	RUN("INT_MAX %%x",        "%x", INT_MAX)
	RUN("INT_MAX %%X",        "%X", INT_MAX)
	RUN("UINT_MAX %%x",       "%x", UINT_MAX)
	RUN("UINT_MAX %%X",       "%X", UINT_MAX)
	RUN("negative -1 %%x",    "%x", -1)
	RUN("negative -1 %%X",    "%X", -1)
	RUN("negative -42 %%x",   "%x", -42)
	RUN("negative -42 %%X",   "%X", -42)
	RUN("INT_MIN %%x",        "%x", INT_MIN)
	RUN("INT_MIN %%X",        "%X", INT_MIN)
	RUN("deadbeef",           "%x", 0xdeadbeef)
	RUN("DEADBEEF",           "%X", 0xdeadbeef)
	RUN("cafebabe %%x",       "%x", 0xcafebabe)
	RUN("cafebabe %%X",       "%X", 0xcafebabe)
	RUN("two %%x",            "%x%x", 255, 16)
	RUN("two %%X",            "%X%X", 255, 16)
	RUN("mixed %%x %%X",      "%x %X", 255, 255)
	RUN("surrounded",         "0x[%x]", 42)
	RUN("mixed %%x %%d",      "%x %d", 255, 255)
	RUN("mixed %%x %%u",      "%x %u", UINT_MAX, UINT_MAX)

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}

void	test_ft_printf_u(void)
{
	int				r1;
	int				r2;
	int				pass;
	int				fail;

	pass = 0;
	fail = 0;
	printf("=== %%u TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("zero",               "%u", 0)
	RUN("basic positive",     "%u", 42)
	RUN("large positive",     "%u", 123456789)
	RUN("UINT_MAX",           "%u", UINT_MAX)
	RUN("UINT_MAX - 1",       "%u", UINT_MAX - 1)
	RUN("INT_MAX",            "%u", INT_MAX)
	RUN("INT_MAX + 1",        "%u", (unsigned int)(INT_MAX) + 1)
	RUN("negative -1",        "%u", -1)
	RUN("negative -42",       "%u", -42)
	RUN("INT_MIN",            "%u", INT_MIN)
	RUN("INT_MIN + 1",        "%u", INT_MIN + 1)
	RUN("two %%u",            "%u%u", 1, 2)
	RUN("surrounded",         "val=[%u]!", 42)
	RUN("mixed %%u %%d",      "%u %d", UINT_MAX, -1)

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}

void	test_ft_printf_percent(void)
{
	int	r1;
	int	r2;
	int	pass;
	int	fail;

	pass = 0;
	fail = 0;
	printf("\n=== %%%% TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("single %%",          "%%")
	RUN("double %%",          "%%%%")
	RUN("surrounded",         "[%%]")
	RUN("%% with %%d",        "%%%d", 42)
	RUN("%% with %%s",        "%%%s", "hi")
	RUN("%% with %%c",        "%%%c", '!')
	RUN("%% before text",     "%%hello")
	RUN("%% after text",      "hello%%")
	RUN("%% between text",    "50%%off")

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}

void	test_ft_printf_c(void)
{
	int	r1;
	int	r2;
	int	pass;
	int	fail;

	pass = 0;
	fail = 0;
	printf("\n=== %%c TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("basic char",        "%c", 'a')
	RUN("uppercase",         "%c", 'A')
	RUN("digit char",        "%c", '0')
	RUN("space",             "%c", ' ')
	RUN("newline char",      "%c", '\n')
	RUN("tab char",          "%c", '\t')
	RUN("null char",         "%c", '\0')
	RUN("del char",          "%c", 127)
	RUN("char 1",            "%c", 1)
	RUN("char 255",          "%c", 255)
	RUN("two %%c",           "%c%c", 'a', 'b')
	RUN("surrounded",        "[%c]", 'x')
	RUN("mixed %%c %%d",     "%c=%d", 'A', 65)
	RUN("mixed %%c %%s",     "%c %s", '!', "hello")

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}

void	test_ft_printf_s(void)
{
	int	r1;
	int	r2;
	int	pass;
	int	fail;

	pass = 0;
	fail = 0;
	printf("\n=== %%s TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("basic string",       "%s", "hello")
	RUN("empty string",       "%s", "")
	RUN("single char",        "%s", "a")
	RUN("with spaces",        "%s", "hello world")
	RUN("numbers as string",  "%s", "12345")
	RUN("special chars",      "%s", "!@#$%^&*()")
	RUN("null string",        "%s", (char *)NULL)
	RUN("two %%s",            "%s%s", "foo", "bar")
	RUN("surrounded",         "val=[%s]!", "test")
	RUN("mixed %%s %%d",      "%s=%d", "answer", 42)
	RUN("long string",        "%s", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
	RUN("newline in str",     "%s", "line1\nline2")
	RUN("tab in str",         "%s", "col1\tcol2")

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}

void	test_ft_printf_d_i(void)
{
	int	r1;
	int	r2;
	int	pass;
	int	fail;

	pass = 0;
	fail = 0;
	printf("\n=== %%d / %%i TESTS ===\n");

#define RUN(label, fmt, ...) \
	printf("  test: %s | fmt: \"%s\"\n", label, fmt); \
	printf("  real: ["); \
	r1 = printf(fmt, ##__VA_ARGS__); \
	printf("]\n  ft:   ["); \
	fflush(stdout); \
	r2 = ft_printf(fmt, ##__VA_ARGS__); \
	printf("]\n"); \
	if (r1 == r2) { printf("  [PASS] ret=%d\n\n", r1); pass++; } \
	else { printf("  [FAIL] real=%d ft=%d\n\n", r1, r2); fail++; }

	RUN("zero",          "%d", 0)
	RUN("positive",      "%d", 42)
	RUN("negative",      "%d", -42)
	RUN("%%i zero",      "%i", 0)
	RUN("%%i positive",  "%i", 42)
	RUN("%%i negative",  "%i", -42)
	RUN("INT_MAX",       "%d", INT_MAX)
	RUN("INT_MIN",       "%d", INT_MIN)
	RUN("%%i INT_MAX",   "%i", INT_MAX)
	RUN("%%i INT_MIN",   "%i", INT_MIN)
	RUN("INT_MIN+1",     "%d", INT_MIN + 1)
	RUN("INT_MAX-1",     "%d", INT_MAX - 1)
	RUN("surrounded",    "val=%d!", 7)
	RUN("two %%d",       "%d%d", 1, 2)
	RUN("mixed",         "%d %i %d", -1, 0, 1)
	RUN("no specifier",  "no specifier")
	RUN("literal %%%%",  "%%")
	RUN("%%d then %%%%", "%d%%", 42)
	RUN("-9",            "%d", -9)
	RUN("-1",            "%d", -1)
	RUN("1",             "%d", 1)
	RUN("9",             "%d", 9)

#undef RUN

	printf("--- %d passed, %d failed ---\n", pass, fail);
}