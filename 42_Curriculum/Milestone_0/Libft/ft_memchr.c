#include "libft.h"

/*
** Searches the first n bytes of the memory area pointed to by s
** for the first occurrence of the byte value c.
**
** Parameters:
** s : pointer to the memory block to search
** c : byte value to locate (converted to unsigned char)
** n : number of bytes to examine
**
** Returns:
** A pointer to the first occurrence of c within the memory block.
** NULL if the value is not found in the first n bytes.
**
** Note:
** The memory is accessed byte-by-byte by casting the pointer to
** unsigned char*, since this is the smallest addressable unit in C.
*/
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	c = (unsigned char)c;
	str = (unsigned char *)s;
	i = 0;
	if (n == 0)
		return (NULL);
	while (i < (n - 1))
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	if (str[i] == c)
		return (&str[i]);
	return (NULL);
}
