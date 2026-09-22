#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include "../../libft.h"

void	ft_putnbr_fd(int n, int fd);

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

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
		printf("  Expected : [%s]\n", expected);
		printf("  Got      : [%s]\n", buf);
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

static void	run_test(const char *tmp, int n, const char *expected, const char *label)
{
	int	fd;

	fd = open_tmpfile(tmp);
	ft_putnbr_fd(n, fd);
	close(fd);
	check_file_content(tmp, expected, label);
}

int	main(void)
{
	const char	*tmp = "/tmp/ft_putnbr_fd_test.txt";

	printf(YELLOW "\n=== ft_putnbr_fd tests ===\n\n" RESET);

	/* --- Normal positive numbers --- */
	printf(YELLOW "[Group 1: Normal positive numbers]\n" RESET);
	run_test(tmp, 0,		"0",		"zero");
	run_test(tmp, 1,		"1",		"one");
	run_test(tmp, 42,		"42",		"42");
	run_test(tmp, 100,		"100",		"100");
	run_test(tmp, 12345,	"12345",	"12345");

	/* --- Negative numbers --- */
	printf(YELLOW "\n[Group 2: Negative numbers]\n" RESET);
	run_test(tmp, -1,		"-1",		"-1");
	run_test(tmp, -42,		"-42",		"-42");
	run_test(tmp, -100,		"-100",		"-100");
	run_test(tmp, -12345,	"-12345",	"-12345");

	/* --- Boundary: INT_MAX / INT_MIN --- */
	printf(YELLOW "\n[Group 3: INT boundaries]\n" RESET);
	run_test(tmp, INT_MAX,	"2147483647",	"INT_MAX");
	run_test(tmp, INT_MIN,	"-2147483648",	"INT_MIN");
	run_test(tmp, INT_MAX - 1, "2147483646",	"INT_MAX - 1");
	run_test(tmp, INT_MIN + 1, "-2147483647",	"INT_MIN + 1");

	/* --- Single digits --- */
	printf(YELLOW "\n[Group 4: All single digits]\n" RESET);
	run_test(tmp, 0, "0", "digit 0");
	run_test(tmp, 1, "1", "digit 1");
	run_test(tmp, 2, "2", "digit 2");
	run_test(tmp, 3, "3", "digit 3");
	run_test(tmp, 4, "4", "digit 4");
	run_test(tmp, 5, "5", "digit 5");
	run_test(tmp, 6, "6", "digit 6");
	run_test(tmp, 7, "7", "digit 7");
	run_test(tmp, 8, "8", "digit 8");
	run_test(tmp, 9, "9", "digit 9");

	/* --- Powers of 10 --- */
	printf(YELLOW "\n[Group 5: Powers of 10]\n" RESET);
	run_test(tmp, 10,		"10",		"10");
	run_test(tmp, 100,		"100",		"100");
	run_test(tmp, 1000,		"1000",		"1000");
	run_test(tmp, 10000,	"10000",	"10000");
	run_test(tmp, 1000000,	"1000000",	"1000000");
	run_test(tmp, -10,		"-10",		"-10");
	run_test(tmp, -1000000,	"-1000000",	"-1000000");

	/* --- Multiple calls accumulate --- */
	printf(YELLOW "\n[Group 6: Multiple calls same fd]\n" RESET);
	{
		int		fd;
		FILE	*f;
		char	buf[4096];
		size_t	n;

		fd = open_tmpfile(tmp);
		ft_putnbr_fd(1, fd);
		ft_putnbr_fd(2, fd);
		ft_putnbr_fd(3, fd);
		close(fd);
		f = fopen(tmp, "r");
		n = fread(buf, 1, sizeof(buf) - 1, f);
		buf[n] = '\0';
		fclose(f);
		if (strcmp(buf, "123") == 0)
			printf(GREEN "[PASS] multiple calls accumulate correctly\n" RESET);
		else
		{
			printf(RED "[FAIL] multiple calls accumulate correctly\n" RESET);
			printf("  Expected : [123]\n");
			printf("  Got      : [%s]\n", buf);
		}
	}

	/* --- stdout visual check --- */
	printf(YELLOW "\n[Group 7: stdout visual check]\n" RESET);
	printf("  ft_putnbr_fd(42, 1)       -> [");
	ft_putnbr_fd(42, STDOUT_FILENO);
	printf("]\n");
	printf("  ft_putnbr_fd(-2147483648, 1) -> [");
	ft_putnbr_fd(-2147483648, STDOUT_FILENO);
	printf("]\n");
	printf("  ft_putnbr_fd(0, 1)         -> [");
	ft_putnbr_fd(0, STDOUT_FILENO);
	printf("]\n");

	/* --- Invalid fd (must not crash) --- */
	printf(YELLOW "\n[Group 8: Invalid fd]\n" RESET);
	printf("  ft_putnbr_fd(42, -1)  -> should not crash\n");
	ft_putnbr_fd(42, -1);
	printf(GREEN "  [PASS] no crash on fd=-1\n" RESET);
	printf("  ft_putnbr_fd(42, 999) -> should not crash\n");
	ft_putnbr_fd(42, 999);
	printf(GREEN "  [PASS] no crash on fd=999\n" RESET);

	printf(YELLOW "\n=== Done ===\n\n" RESET);
	return (0);
}