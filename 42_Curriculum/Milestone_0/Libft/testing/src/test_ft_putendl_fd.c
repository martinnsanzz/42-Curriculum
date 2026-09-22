#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "../../libft.h"

void	ft_putendl_fd(char *s, int fd);

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define YELLOW "\033[0;33m"

static void	check_file_content(const char *filename, const char *expected, const char *test_name)
{
	FILE	*f;
	char	buf[4096];
	size_t	n;

	f = fopen(filename, "r");
	if (!f)
	{
		printf(RED "[FAIL] %s: could not open temp file\n" RESET, test_name);
		return ;
	}
	n = fread(buf, 1, sizeof(buf) - 1, f);
	buf[n] = '\0';
	fclose(f);
	if (strcmp(buf, expected) == 0)
		printf(GREEN "[PASS] %s\n" RESET, test_name);
	else
	{
		printf(RED "[FAIL] %s\n" RESET, test_name);
		printf("  Expected : ");
		for (size_t i = 0; expected[i]; i++)
			expected[i] == '\n' ? printf("\\n") : printf("%c", expected[i]);
		printf("\n  Got      : ");
		for (size_t i = 0; i < n; i++)
			buf[i] == '\n' ? printf("\\n") : printf("%c", buf[i]);
		printf("\n");
	}
}

static int	open_tmpfile(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}

int	main(void)
{
	int		fd;
	char	*tmp;

	tmp = "/tmp/ft_putendl_fd_test.txt";

	printf(YELLOW "\n=== ft_putendl_fd tests ===\n\n" RESET);

	/* --- Normal strings --- */
	printf(YELLOW "[Group 1: Normal strings]\n" RESET);

	fd = open_tmpfile(tmp);
	ft_putendl_fd("hello", fd);
	close(fd);
	check_file_content(tmp, "hello\n", "basic string");

	fd = open_tmpfile(tmp);
	ft_putendl_fd("42", fd);
	close(fd);
	check_file_content(tmp, "42\n", "numeric string");

	fd = open_tmpfile(tmp);
	ft_putendl_fd("Hello, World!", fd);
	close(fd);
	check_file_content(tmp, "Hello, World!\n", "string with punctuation");

	fd = open_tmpfile(tmp);
	ft_putendl_fd("a b c", fd);
	close(fd);
	check_file_content(tmp, "a b c\n", "string with spaces");

	/* --- Edge: empty string --- */
	printf(YELLOW "\n[Group 2: Edge cases]\n" RESET);

	fd = open_tmpfile(tmp);
	ft_putendl_fd("", fd);
	close(fd);
	check_file_content(tmp, "\n", "empty string -> only newline");

	fd = open_tmpfile(tmp);
	ft_putendl_fd(" ", fd);
	close(fd);
	check_file_content(tmp, " \n", "single space");

	fd = open_tmpfile(tmp);
	ft_putendl_fd("\t", fd);
	close(fd);
	check_file_content(tmp, "\t\n", "tab character");

	fd = open_tmpfile(tmp);
	ft_putendl_fd("\n", fd);
	close(fd);
	check_file_content(tmp, "\n\n", "string is newline -> two newlines");

	/* --- Special characters --- */
	printf(YELLOW "\n[Group 3: Special characters]\n" RESET);

	fd = open_tmpfile(tmp);
	ft_putendl_fd("!@#$%^&*()", fd);
	close(fd);
	check_file_content(tmp, "!@#$%^&*()\n", "special chars");

	fd = open_tmpfile(tmp);
	ft_putendl_fd("abc\0def", fd);
	close(fd);
	check_file_content(tmp, "abc\n", "string with embedded null (stops at \\0)");

	/* --- Multiple calls / fd behavior --- */
	printf(YELLOW "\n[Group 4: Multiple calls / fd behavior]\n" RESET);

	fd = open_tmpfile(tmp);
	ft_putendl_fd("line1", fd);
	ft_putendl_fd("line2", fd);
	ft_putendl_fd("line3", fd);
	close(fd);
	check_file_content(tmp, "line1\nline2\nline3\n", "multiple calls same fd");

	/* --- stdout / stderr (visual check) --- */
	printf(YELLOW "\n[Group 5: stdout / stderr (visual)]\n" RESET);

	printf("  stdout (fd=1): expect 'to_stdout\\n':\n  -> ");
	ft_putendl_fd("to_stdout", STDOUT_FILENO);

	fprintf(stderr, "  stderr (fd=2): expect 'to_stderr\\n':\n  -> ");
	ft_putendl_fd("to_stderr", STDERR_FILENO);

	/* --- Invalid fd (should not crash) --- */
	printf(YELLOW "\n[Group 6: Invalid fd]\n" RESET);
	printf("  Calling ft_putendl_fd(\"test\", -1) -> should not crash\n");
	ft_putendl_fd("test", -1);
	printf(GREEN "  [PASS] no crash on fd=-1\n" RESET);

	printf("  Calling ft_putendl_fd(\"test\", 999) -> should not crash\n");
	ft_putendl_fd("test", 999);
	printf(GREEN "  [PASS] no crash on fd=999\n" RESET);

	printf(YELLOW "\n=== Done ===\n\n" RESET);
	return (0);
}