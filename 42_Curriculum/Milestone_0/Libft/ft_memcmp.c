#include "libft.h"

/*
** Compares the first n bytes of two memory blocks.
**
** Parameters:
** s1 : pointer to the first memory block
** s2 : pointer to the second memory block
** n  : number of bytes to compare
**
** Returns:
** An integer representing the difference between the first pair
** of differing bytes:
**   - positive if the byte in s1 is greater than the byte in s2
**   - negative if the byte in s1 is less than the byte in s2
**   - 0 if the first n bytes of both memory blocks are identical
**
** Note:
** The memory is compared byte-by-byte by casting the pointers
** to unsigned char*. This ensures comparisons use values in the
** range 0–255 instead of potentially signed char values.
*/
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < (n - 1))
	{
		if (str1[i] != str2[i])
			return ((unsigned char)str1[i] - (unsigned char)str2[i]);
		i++;
	}
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}
