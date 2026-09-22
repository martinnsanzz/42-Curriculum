#include "libft.h"

/*
** Null terminates '\0' the first n bytes of the memory area pointed to by s
** 
** Parameters:
** s : pointer to the memory block to modify
** n : number of bytes to write
**
** Note:
** Memory is written byte-by-byte, therefore the pointer is cast
** to unsigned char* to allow single-byte access.
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
