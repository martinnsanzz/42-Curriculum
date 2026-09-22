#include "libft.h"

/*
** Compares up to n characters of the strings s1 and s2.
**
** The comparison stops when:
** - n characters have been compared, or
** - a null terminator is encountered in either string.
**
** Parameters:
** s1 : first null-terminated string
** s2 : second null-terminated string
** n : maximum number of characters to compare
**
** Returns:
** An integer representing the difference between the first pair
** of differing characters:
**   - positive if s1 > s2
**   - negative if s1 < s2
**   - 0 if the compared characters are equal
**
** Notes:
** Characters are cast to unsigned char before subtraction to
** ensure comparisons are done using their numeric byte values.
*/
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < (n - 1) && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
