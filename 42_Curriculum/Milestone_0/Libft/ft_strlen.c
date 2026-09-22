#include "libft.h"

/*
** Computes the length of a null-terminated string.
** Parameters:
** s	: null-terminated string to measure
**
** Returns:
** Number of characters in the string, excluding the terminating '\0'.
*/
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}
