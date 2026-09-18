/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 15:02:34 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Compares two null-terminated strings byte by byte.
 *
 * @param s1 First string to compare.
 * @param s2 Second string to compare.
 *
 * @return 0 if the strings are equal, a negative value if @p s1 sorts
 *         before @p s2, a positive value if @p s1 sorts after @p s2.
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/**
 * @brief Computes the length of a null-terminated string.
 *
 * @param s String to measure.
 *
 * @return Number of characters before the terminating null byte.
 */
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}
