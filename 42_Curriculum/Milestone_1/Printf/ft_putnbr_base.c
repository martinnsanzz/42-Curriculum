#include "ft_printf.h"

static int	check_base_error(const char *base);
static void	convert_base(unsigned long num, const char *base,
				unsigned long base_size, int *len);

/**
 * @brief  Writes an unsigned long integer to stdout in the given base and updates the length counter.
 *
 * @details  Guards against NULL `len` and `base` pointers, and validates the base
 *           string via `check_base_error` — returning early if any check fails.
 *           Computes the base size by iterating over the base string until '\0'.
 *           Delegates the actual recursive conversion and output to `convert_base`,
 *           which is also responsible for updating `len`.
 *
 * @param  num   unsigned long  — The non-negative integer to convert and print.
 *                                A value of 0 is valid and should output the first
 *                                character of `base`. [INFERRED]
 * @param  base  const char*    — A null-terminated string of unique characters
 *                                representing the target base (e.g. "0123456789abcdef"
 *                                for base 16). Must not be NULL. Validated by
 *                                `check_base_error` before use.
 * @param  len   int*           — Pointer to the running output length counter. Updated
 *                                by `convert_base` for each digit written. Must not
 *                                be NULL.
 */
void	ft_putnbr_base(unsigned long num, const char *base, int *len)
{
	unsigned long	base_size;

	if (!len || !base || check_base_error(&base[0]))
		return ;
	base_size = 0;
	while (base[base_size] != '\0')
		base_size++;
	convert_base(num, &base[0], base_size, len);
}

/**
 * @brief  Recursively converts and writes an unsigned long in the given base to stdout.
 *
 * @details  Recursively divides `num` by `base_size` until the value falls below
 *           `base_size`, unwinding the call stack to print digits in most-significant
 *           to least-significant order. `ft_putchar_fd` and `*len += 1` are called
 *           unconditionally on every stack frame during unwinding, correctly counting
 *           every digit written.
 */
static void	convert_base(unsigned long num, const char *base,
	unsigned long base_size, int *len)
{
	if (num >= (unsigned long)base_size)
	{
		convert_base(num / base_size, &base[0], base_size, len);
		num = num % base_size;
	}
	ft_putchar_fd(base[num % base_size], 1);
	*len += 1;
}

/**
 * @brief  Validates a base string for use in numeric conversion.
 *
 * @return int — 0 if the base is valid and safe to use for conversion.
 *               1 if the base is invalid or base == NULL
 */
static int	check_base_error(const char *base)
{
	int	j;
	int	i;

	if (!base)
		return (1);
	i = 0;
	while (base[i] != '\0')
	{
		if (base[i] == ' ' || (base[i] >= 9 && base[i] <= 13) || base[i] == '+'
			|| base[i] == '-')
			return (1);
		j = i + 1;
		while (base[j] != '\0')
		{
			if (base[i] == base[j])
				return (1);
			j++;
		}
		i++;
	}
	if (i < 2)
		return (1);
	return (0);
}
