#include "libft.h"

static size_t	num_len(int num);

/*
** Converts an integer to its string representation.
**
** Parameters:
** n : the integer to convert
**
** Returns:
** A pointer to a newly allocated string containing the integer representation,
** or NULL if memory allocation fails.
**
** Note:
** The returned string must be freed by the caller.
** Negative integers are represented with a leading minus sign.
*/
char	*ft_itoa(int n)
{
	char	*num;
	long	nb;
	size_t	len;

	nb = n;
	len = num_len(nb) + (nb < 0);
	num = ft_calloc(len + 1, sizeof(char));
	if (num == NULL)
		return (NULL);
	if (nb < 0)
	{
		num[0] = '-';
		nb *= -1;
	}
	while (len > (n < 0))
	{
		num[--len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (num);
}

/*
** Calculates the number of digits needed to represent an integer.
**
** Parameters:
** num : the integer to analyze
**
** Returns:
** The count of digits required to represent the integer (excluding the sign).
**
** Note:
** For negative numbers, the function converts to positive before counting,
** so the sign is not included in the returned length.
*/
static size_t	num_len(int num)
{
	size_t	len;
	long	nbum;

	len = 1;
	nbum = num;
	if (nbum < 0)
		nbum *= -1;
	while (nbum >= 10)
	{
		len++;
		nbum /= 10;
	}
	return (len);
}
