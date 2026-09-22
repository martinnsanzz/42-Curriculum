#include "libft.h"

/*
** Converts the initial portion of a string to an integer.
**
** Parameters:
**  nptr : null-terminated string containing the number to convert
**
** Returns:
**  The integer value represented at the beginning of the string.
**
** Behavior:
**  - Leading whitespace characters are skipped
**  - An optional '+' or '-' sign is supported
**  - Conversion stops at the first non-digit character
**
** Note:
**  This function does not detect overflow or invalid input.
*/
int	ft_atoi(const char *nptr)
{
	int	sign;
	int	num;

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
	while (*nptr >= '0' && *nptr <= '9')
	{
		num = num * 10 + (*nptr - '0');
		nptr++;
	}
	return (num * sign);
}
