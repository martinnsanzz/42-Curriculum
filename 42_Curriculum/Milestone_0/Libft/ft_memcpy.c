#include "libft.h"

/*
** Copies the first n bytes of the memory area pointed to by src
** to dest.
**
** Parameters:
** dest : pointer to the memory block to copy
** src : pointer to the memory block to copy from
** n : number of bytes to copy
**
** Returns:
** The modified dest pointer.
**
** Note:
** Memory is written byte-by-byte, therefore the pointer is cast
** to unsigned char* to allow single-byte access.
*/
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptr_dest;
	unsigned char	*ptr_src;

	ptr_dest = (unsigned char *)dest;
	ptr_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		ptr_dest[i] = ptr_src[i];
		i++;
	}
	return (dest);
}
