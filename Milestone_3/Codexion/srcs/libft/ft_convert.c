/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:45:45 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 15:01:26 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static size_t	num_len(int num);

/**
 * @brief Converts a string to a long long integer, skipping leading
 *        whitespace and handling an optional sign.
 *
 * @param nptr String to convert (e.g. "  -42abc").
 *
 * @return The parsed integer value, or 0 if the parsed value exceeds INT_MAX.
 */
long long    ft_atoi(const char *nptr)
{
    long long    num;
    int            sign;

    num = 0;
    sign = 1;
    while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
        nptr++;
    if (*nptr == '+' || *nptr == '-')
    {
        if (*nptr == '-')
            sign = -1;
        nptr++;
    }
    while (*nptr >= '0' && *nptr <= '9')
    {
        num = num * 10 + (*nptr - '0');
        nptr++;
    }
    if (num > INT_MAX)
        return (0);
    return (num * sign);
}

/**
 * @brief Converts an integer to a newly allocated string.
 *
 * @param n Integer to convert.
 *
 * @return Heap-allocated string representation of @p n, or NULL if
 *         allocation fails. Caller is responsible for freeing it.
 */
char    *ft_itoa(int n)
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

/**
 * @brief Computes the number of digits in the absolute value of @p num.
 *
 * @param num Integer whose digit count is measured (sign is ignored).
 *
 * @return Number of digits (always at least 1, e.g. for 0).
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