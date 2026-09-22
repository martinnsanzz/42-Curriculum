#include "libft.h"

/*
** Fills the first n bytes of the memory area pointed to by s
** with the byte value c.
**
** Parameters:
** s : pointer to the memory block to modify
** c : value used to fill the memory (converted to unsigned char)
** n : number of bytes to write
**
** Returns:
** The original pointer s.
**
** Note:
** Memory is written byte-by-byte, therefore the pointer is cast
** to unsigned char* to allow single-byte access.
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
