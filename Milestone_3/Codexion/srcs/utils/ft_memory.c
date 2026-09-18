/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 15:02:04 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Sets the first @p n bytes of memory area @p s to zero.
 *
 * @param s Pointer to the memory area to zero out.
 * @param n Number of bytes to set to zero.
 */
void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = '\0';
		i++;
	}
}

/**
 * @brief Allocates memory for an array of @p nmemb elements of @p size
 *        bytes each, and zero-initializes it.
 *
 * @param nmemb Number of elements.
 * @param size  Size in bytes of each element.
 *
 * @return Pointer to the zero-initialized allocated memory, or NULL if
 *         allocation fails or if @p nmemb * @p size would overflow.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}

/**
 * @brief Fills the first @p n bytes of memory area @p s with the
 *        constant byte @p c.
 *
 * @param s Pointer to the memory area to fill.
 * @param c Byte value to set (passed as int, converted to unsigned char).
 * @param n Number of bytes to fill.
 *
 * @return Pointer to the memory area @p s.
 */
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	c_char;
	unsigned char	*ptr;

	c_char = (unsigned char)c;
	ptr = (unsigned char *)s;
	while (n--)
		*ptr++ = c_char;
	return (s);
}