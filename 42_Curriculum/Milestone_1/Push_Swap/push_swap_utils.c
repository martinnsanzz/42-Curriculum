#include "push_swap.h"

/**
 * @brief  Converts a string to a long integer with strict format validation.
 *
 * @details  Skips leading whitespace (spaces and ASCII 9–13), then parses an
 *           optional sign character. Calls `print_error` and exits
 *           if the first non-sign character is not a digit, or if any non-digit
 *           character is found after the numeric sequence. Returns the parsed
 *           value with the correct sign applied.
 *
 * @return long — The parsed integer value on success, with sign applied.
 *                Never returns on invalid input.
 */
long	ft_atoi_strict(const char *nptr)
{
	int		sign;
	long	num;

	sign = 1;
	num = 0;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	if (!ft_isdigit(*nptr))
		print_error();
	while (*nptr >= '0' && *nptr <= '9')
	{
		num = num * 10 + (*nptr - '0');
		nptr++;
	}
	if (*nptr != '\0')
		print_error();
	return (num * sign);
}

/**
 * @brief  Compares two null-terminated strings lexicographically.
 *
 * @details  Iterates through both strings simultaneously until a differing
 *           character is found or both strings reach '\0'.
 * 
 * @return int — 0 if both strings are equal.
 *               Positive value if the differing character in s1 > s2.
 *               Negative value if the differing character in s1 < s2.
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/**
 * @brief When called it prints 'Error' to stderr (2) and 
 * 		  exits the program. The user decides when to call
 * 		  this function.
 */
void	print_error(void)
{
	ft_putstr_fd("Error\n", 2);
	exit (1);
}

/**
 * @brief Sums up all the actions used to sort out the list of numbers
 * 
 * @return The total sum of the actions
 */
int	sum_operations(t_operations op)
{
	int	total_operations;
	int	swap;
	int	push;
	int	rotate;
	int	rev_rotate;

	swap = op.sa + op.sb + op.ss;
	push = op.pa + op.pb;
	rotate = op.ra + op.rb + op.rr;
	rev_rotate = op.rra + op.rrb + op.rrr;
	total_operations = swap + push + rotate + rev_rotate;
	return (total_operations);
}

/**
 * @brief  Computes the ceiling integer square root of a non-negative integer.
 *
 * @details  Uses the property that the sum of the first n odd numbers equals n².
 *           Repeatedly subtracts successive odd numbers from `nb`, incrementing
 *           the root counter each time. If `nb` reaches exactly 0, the input was
 *           a perfect square and `res` is returned directly. If the loop exits
 *           with `nb > 0`, the input was not a perfect square and `res + 1` is
 *           returned as the ceiling.
 *
 * @return int — Ceiling of sqrt(nb). Returns 0 for negative input or 0.
 */
int	ft_sqrt(int nb)
{
	int	res;
	int	odd;

	odd = 1;
	res = 0;
	if (nb <= 0)
		return (res);
	while (odd <= nb)
	{
		nb -= odd;
		res++;
		odd += 2;
	}
	if (nb != 0)
		return (res + 1);
	return (res);
}
