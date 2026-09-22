#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../libft.h"

/* ==================== TEST RUNNER ==================== */

static int	g_pass = 0;
static int	g_fail = 0;

static void	check(const char *desc, char result, char expected)
{
	printf("  TEST: %s\n", desc);
	printf("    Expected: '%c' (dec: %d)\n", expected, (unsigned char)expected);
	printf("    Got:      '%c' (dec: %d)\n", result, (unsigned char)result);
	if (result == expected)
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

/*
** Write to a tmp file, read back, compare
** Returns the char read, or 0 on error
*/
static char	capture_fd(char c, int fd_write, int fd_read)
{
	char	buf[2];

	buf[0] = 0;
	buf[1] = 0;
	ft_putchar_fd(c, fd_write);
	lseek(fd_read, 0, SEEK_SET);
	if (read(fd_read, buf, 1) != 1)
		return (0);
	return (buf[0]);
}

/* ==================== TESTS ==================== */

static void	test_stdout(void)
{
	printf("=== STDOUT (fd=1) ===\n\n");
	printf("  TEST: ft_putchar_fd('A', 1)\n");
	printf("    Expected output on stdout: A\n");
	printf("    Got:      ");
	ft_putchar_fd('A', 1);
	printf("\n    [MANUAL CHECK]\n\n");

	printf("  TEST: ft_putchar_fd('\\n', 1)\n");
	printf("    Expected: newline on stdout\n");
	printf("    Got:      [");
	ft_putchar_fd('\n', 1);
	printf("]\n    [MANUAL CHECK]\n\n");
}

static void	test_stderr(void)
{
	printf("=== STDERR (fd=2) ===\n\n");
	printf("  TEST: ft_putchar_fd('E', 2)\n");
	printf("    Expected: 'E' written to stderr\n");
	printf("    Got (stderr): ");
	ft_putchar_fd('E', 2);
	fprintf(stderr, "\n");
	printf("    [MANUAL CHECK]\n\n");
}

static void	test_normal_chars(void)
{
	int		fd[2];
	char	result;
	int		i;
	char	cases[] = "abcdefghijklmnopqrstuvwxyz";

	printf("=== NORMAL CHARS (via tmp file) ===\n\n");
	i = 0;
	while (cases[i])
	{
		fd[0] = open("/tmp/ft_putchar_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
		fd[1] = open("/tmp/ft_putchar_test", O_RDONLY);
		if (fd[0] < 0 || fd[1] < 0)
		{
			printf("  [ERROR] Could not open tmp file\n");
			return ;
		}
		result = capture_fd(cases[i], fd[0], fd[1]);
		char desc[64];
		snprintf(desc, sizeof(desc), "char '%c'", cases[i]);
		check(desc, result, cases[i]);
		close(fd[0]);
		close(fd[1]);
		i++;
	}
}

static void	test_uppercase(void)
{
	int		fd[2];
	char	result;
	int		i;
	char	cases[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	printf("=== UPPERCASE CHARS ===\n\n");
	i = 0;
	while (cases[i])
	{
		fd[0] = open("/tmp/ft_putchar_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
		fd[1] = open("/tmp/ft_putchar_test", O_RDONLY);
		if (fd[0] < 0 || fd[1] < 0)
		{
			printf("  [ERROR] Could not open tmp file\n");
			return ;
		}
		result = capture_fd(cases[i], fd[0], fd[1]);
		char desc[64];
		snprintf(desc, sizeof(desc), "char '%c'", cases[i]);
		check(desc, result, cases[i]);
		close(fd[0]);
		close(fd[1]);
		i++;
	}
}

static void	test_digits(void)
{
	int		fd[2];
	char	result;
	int		i;
	char	cases[] = "0123456789";

	printf("=== DIGIT CHARS ===\n\n");
	i = 0;
	while (cases[i])
	{
		fd[0] = open("/tmp/ft_putchar_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
		fd[1] = open("/tmp/ft_putchar_test", O_RDONLY);
		if (fd[0] < 0 || fd[1] < 0)
		{
			printf("  [ERROR] Could not open tmp file\n");
			return ;
		}
		result = capture_fd(cases[i], fd[0], fd[1]);
		char desc[64];
		snprintf(desc, sizeof(desc), "digit '%c'", cases[i]);
		check(desc, result, cases[i]);
		close(fd[0]);
		close(fd[1]);
		i++;
	}
}

static void	test_special_chars(void)
{
	int		fd[2];
	char	result;

	printf("=== SPECIAL / PRINTABLE CHARS ===\n\n");

	char	cases[] = {' ', '!', '@', '#', '$', '%', '^', '&',
		'*', '(', ')', '-', '+', '=', '[', ']',
		'{', '}', '|', ';', ':', '\'', '"', ',',
		'.', '<', '>', '/', '?', '`', '~', '\\'};
	int		len = sizeof(cases);
	int		i = 0;

	while (i < len)
	{
		fd[0] = open("/tmp/ft_putchar_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
		fd[1] = open("/tmp/ft_putchar_test", O_RDONLY);
		if (fd[0] < 0 || fd[1] < 0)
		{
			printf("  [ERROR] Could not open tmp file\n");
			return ;
		}
		result = capture_fd(cases[i], fd[0], fd[1]);
		char desc[64];
		snprintf(desc, sizeof(desc), "special char (dec %d)", (unsigned char)cases[i]);
		check(desc, result, cases[i]);
		close(fd[0]);
		close(fd[1]);
		i++;
	}
}

static void	test_edge_cases(void)
{
	int		fd[2];
	char	result;

	printf("=== EDGE CASES ===\n\n");

	char	edges[] = {0, 1, 127, -1, -128};
	char	*descs[] = {
		"NUL char (0)",
		"char dec 1 (SOH)",
		"char dec 127 (DEL)",
		"char -1 (dec 255 unsigned)",
		"char -128 (dec 128 unsigned)"
	};
	int		i = 0;

	while (i < 5)
	{
		fd[0] = open("/tmp/ft_putchar_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
		fd[1] = open("/tmp/ft_putchar_test", O_RDONLY);
		if (fd[0] < 0 || fd[1] < 0)
		{
			printf("  [ERROR] Could not open tmp file\n");
			return ;
		}
		result = capture_fd(edges[i], fd[0], fd[1]);
		check(descs[i], result, edges[i]);
		close(fd[0]);
		close(fd[1]);
		i++;
	}
}

static void	test_invalid_fd(void)
{
	printf("=== INVALID FD ===\n\n");
	printf("  TEST: ft_putchar_fd('A', -1)\n");
	printf("    Expected: no crash, write fails silently\n");
	ft_putchar_fd('A', -1);
	printf("    [PASS] -- no crash\n\n");
	g_pass++;

	printf("  TEST: ft_putchar_fd('A', 999)\n");
	printf("    Expected: no crash, write fails silently\n");
	ft_putchar_fd('A', 999);
	printf("    [PASS] -- no crash\n\n");
	g_pass++;
}

static void	test_only_one_byte_written(void)
{
	int		fd[2];
	char	buf[4];
	off_t	size;

	printf("=== ONLY ONE BYTE WRITTEN ===\n\n");

	fd[0] = open("/tmp/ft_putchar_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
	fd[1] = open("/tmp/ft_putchar_test", O_RDONLY);
	if (fd[0] < 0 || fd[1] < 0)
	{
		printf("  [ERROR] Could not open tmp file\n");
		return ;
	}
	ft_putchar_fd('Z', fd[0]);
	size = lseek(fd[1], 0, SEEK_END);
	printf("  TEST: exactly 1 byte written for 'Z'\n");
	printf("    Expected size: 1\n");
	printf("    Got size:      %lld\n", (long long)size);
	if (size == 1)
	{
		lseek(fd[1], 0, SEEK_SET);
		read(fd[1], buf, 4);
		printf("    Byte value: dec %d\n", (unsigned char)buf[0]);
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL]\n\n");
		g_fail++;
	}
	close(fd[0]);
	close(fd[1]);
}

/* ==================== MAIN ==================== */

int	main(void)
{
	printf("\n========================================\n");
	printf("       TESTER: ft_putchar_fd\n");
	printf("========================================\n\n");

	test_stdout();
	test_stderr();
	test_normal_chars();
	test_uppercase();
	test_digits();
	test_special_chars();
	test_edge_cases();
	test_invalid_fd();
	test_only_one_byte_written();

	printf("========================================\n");
	printf("  RESULTS: %d passed | %d failed\n", g_pass, g_fail);
	printf("========================================\n\n");
	unlink("/tmp/ft_putchar_test");
	return (g_fail > 0 ? 1 : 0);
}