#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../../libft.h"

/* ==================== TEST RUNNER ==================== */
 
static int	g_pass = 0;
static int	g_fail = 0;
 
static int	capture_and_compare(char *s, const char *expected, size_t expected_len)
{
	int		fd_w;
	int		fd_r;
	char	buf[4096];
	ssize_t	bytes_read;
 
	fd_w = open("/tmp/ft_putstr_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
	fd_r = open("/tmp/ft_putstr_test", O_RDONLY);
	if (fd_w < 0 || fd_r < 0)
	{
		printf("  [ERROR] Could not open tmp file\n");
		if (fd_w >= 0)
			close(fd_w);
		if (fd_r >= 0)
			close(fd_r);
		return (0);
	}
	ft_putstr_fd(s, fd_w);
	lseek(fd_r, 0, SEEK_SET);
	memset(buf, 0, sizeof(buf));
	bytes_read = read(fd_r, buf, sizeof(buf) - 1);
	close(fd_w);
	close(fd_r);
	if (bytes_read < 0)
		return (0);
	if ((size_t)bytes_read != expected_len)
		return (0);
	return (memcmp(buf, expected, expected_len) == 0);
}
 
static void	check(const char *desc, char *input,
		const char *expected, size_t expected_len)
{
	int	ok;
 
	printf("  TEST: %s\n", desc);
	printf("    Input:    \"%s\"\n", input);
	printf("    Expected: \"%s\" (%zu bytes)\n", expected, expected_len);
	ok = capture_and_compare(input, expected, expected_len);
	if (ok)
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
 
static off_t	get_written_size(char *s)
{
	int		fd_w;
	int		fd_r;
	off_t	size;
 
	fd_w = open("/tmp/ft_putstr_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
	fd_r = open("/tmp/ft_putstr_test", O_RDONLY);
	if (fd_w < 0 || fd_r < 0)
	{
		if (fd_w >= 0)
			close(fd_w);
		if (fd_r >= 0)
			close(fd_r);
		return (-1);
	}
	ft_putstr_fd(s, fd_w);
	size = lseek(fd_r, 0, SEEK_END);
	close(fd_w);
	close(fd_r);
	return (size);
}
 
/* ==================== TESTS ==================== */
 
static void	test_stdout(void)
{
	printf("=== STDOUT (fd=1) ===\n\n");
	printf("  TEST: ft_putstr_fd(\"Hello, World!\", 1)\n");
	printf("    Expected on stdout: Hello, World!\n");
	printf("    Got:      ");
	ft_putstr_fd("Hello, World!", 1);
	printf("\n    [MANUAL CHECK]\n\n");
 
	printf("  TEST: ft_putstr_fd(\"42\", 1)\n");
	printf("    Expected on stdout: 42\n");
	printf("    Got:      ");
	ft_putstr_fd("42", 1);
	printf("\n    [MANUAL CHECK]\n\n");
}
 
static void	test_stderr(void)
{
	printf("=== STDERR (fd=2) ===\n\n");
	printf("  TEST: ft_putstr_fd(\"error msg\", 2)\n");
	printf("    Expected on stderr: error msg\n");
	fprintf(stderr, "    Got (stderr): ");
	ft_putstr_fd("error msg", 2);
	fprintf(stderr, "\n");
	printf("    [MANUAL CHECK]\n\n");
}
 
static void	test_normal_strings(void)
{
	printf("=== NORMAL STRINGS ===\n\n");
 
	check("simple lowercase",
		"hello", "hello", 5);
	check("simple uppercase",
		"WORLD", "WORLD", 5);
	check("alphanumeric",
		"abc123", "abc123", 6);
	check("sentence with spaces",
		"the quick brown fox", "the quick brown fox", 19);
	check("single char",
		"a", "a", 1);
	check("single digit",
		"9", "9", 1);
}
 
static void	test_special_chars(void)
{
	printf("=== SPECIAL CHARS ===\n\n");
 
	check("symbols",
		"!@#$%^&*()", "!@#$%^&*()", 10);
	check("spaces only",
		"   ", "   ", 3);
	check("tab char",
		"\t", "\t", 1);
	check("newline char",
		"\n", "\n", 1);
	check("mixed special",
		"hello\nworld\t!", "hello\nworld\t!", 13);
	check("backslash",
		"a\\b", "a\\b", 3);
}
 
static void	test_empty_string(void)
{
	off_t	size;
 
	printf("=== EMPTY STRING ===\n\n");
 
	size = get_written_size("");
	printf("  TEST: ft_putstr_fd(\"\", fd) writes 0 bytes\n");
	printf("    Expected size: 0\n");
	printf("    Got size:      %lld\n", (long long)size);
	if (size == 0)
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
 
static void	test_no_null_terminator_written(void)
{
	off_t	size;
	char	*s = "hello";
 
	printf("=== NUL TERMINATOR NOT WRITTEN ===\n\n");
 
	size = get_written_size(s);
	printf("  TEST: \"hello\" writes exactly 5 bytes (no '\\0')\n");
	printf("    Expected size: 5\n");
	printf("    Got size:      %lld\n", (long long)size);
	if (size == 5)
	{
		printf("    [PASS]\n\n");
		g_pass++;
	}
	else
	{
		printf("    [FAIL] -- wrote %lld bytes, check for extra '\\0'\n\n",
			(long long)size);
		g_fail++;
	}
}
 
static void	test_long_string(void)
{
	char	buf[1025];
	off_t	size;
 
	printf("=== LONG STRING ===\n\n");
 
	memset(buf, 'A', 1024);
	buf[1024] = '\0';
	size = get_written_size(buf);
	printf("  TEST: 1024 x 'A' writes exactly 1024 bytes\n");
	printf("    Expected size: 1024\n");
	printf("    Got size:      %lld\n", (long long)size);
	if (size == 1024)
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
 
static void	test_byte_by_byte_content(void)
{
	int		fd_w;
	int		fd_r;
	char	buf[16];
	char	*s = "abcde";
 
	printf("=== BYTE-BY-BYTE CONTENT VERIFY ===\n\n");
 
	fd_w = open("/tmp/ft_putstr_test", O_RDWR | O_CREAT | O_TRUNC, 0644);
	fd_r = open("/tmp/ft_putstr_test", O_RDONLY);
	if (fd_w < 0 || fd_r < 0)
	{
		printf("  [ERROR] Could not open tmp file\n");
		if (fd_w >= 0) close(fd_w);
		if (fd_r >= 0) close(fd_r);
		return ;
	}
	ft_putstr_fd(s, fd_w);
	lseek(fd_r, 0, SEEK_SET);
	memset(buf, 0, sizeof(buf));
	read(fd_r, buf, 5);
	close(fd_w);
	close(fd_r);
 
	printf("  TEST: \"abcde\" byte-by-byte\n");
	int i = 0;
	int all_ok = 1;
	while (i < 5)
	{
		printf("    buf[%d] expected '%c' (dec %d) | got '%c' (dec %d) %s\n",
			i, s[i], (unsigned char)s[i],
			buf[i], (unsigned char)buf[i],
			buf[i] == s[i] ? "OK" : "MISMATCH");
		if (buf[i] != s[i])
			all_ok = 0;
		i++;
	}
	if (all_ok)
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
 
static void	test_invalid_fd(void)
{
	printf("=== INVALID FD ===\n\n");
 
	printf("  TEST: ft_putstr_fd(\"hello\", -1) -- no crash expected\n");
	ft_putstr_fd("hello", -1);
	printf("    [PASS] -- no crash\n\n");
	g_pass++;
 
	printf("  TEST: ft_putstr_fd(\"hello\", 999) -- no crash expected\n");
	ft_putstr_fd("hello", 999);
	printf("    [PASS] -- no crash\n\n");
	g_pass++;
}
 
static void	test_null_input(void)
{
	pid_t	pid;
	int		status;

	printf("=== NULL INPUT ===\n\n");
	printf("  TEST: ft_putstr_fd(NULL, 1)\n");

	pid = fork();
	if (pid == 0)
	{
		ft_putstr_fd(NULL, 1);
		_exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		printf("    Got:      CRASH (signal %d)\n", WTERMSIG(status));
		printf("    Expected: no crash -- missing NULL guard\n");
		printf("    [FAIL] -- add 'if (!s) return;'\n\n");
		g_fail++;
	}
	else
	{
		printf("    Got:      no crash\n");
		printf("    [PASS]\n\n");
		g_pass++;
	}
}
 
/* ==================== MAIN ==================== */
 
int	main(void)
{
	printf("\n========================================\n");
	printf("       TESTER: ft_putstr_fd\n");
	printf("========================================\n\n");
 
	test_stdout();
	test_stderr();
	test_normal_strings();
	test_special_chars();
	test_empty_string();
	test_no_null_terminator_written();
	test_long_string();
	test_byte_by_byte_content();
	test_invalid_fd();
	test_null_input();
 
	printf("========================================\n");
	printf("  RESULTS: %d passed | %d failed\n", g_pass, g_fail);
	printf("========================================\n\n");
	unlink("/tmp/ft_putstr_test");
	return (g_fail > 0 ? 1 : 0);
}