#include "libft.h"

/*
** Copies n bytes from src to dest, handling overlapping memory regions safely.
**
** Parameters:
** dest : pointer to the destination memory area
** src  : pointer to the source memory area
** n    : number of bytes to copy
**
** Returns:
** A pointer to dest.
**
** Note:
** Unlike ft_memcpy, this function is safe for overlapping memory regions.
** Passing NULL for dest or src with n > 0 is undefined behavior.
*/
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptr_dest;
	unsigned char	*ptr_src;

	ptr_dest = (unsigned char *)dest;
	ptr_src = (unsigned char *)src;
	i = 0;
	if (ptr_dest < ptr_src)
	{
		while (i < n)
		{
			ptr_dest[i] = ptr_src[i];
			i++;
		}
	}
	else
	{
		while (i < n)
		{
			ptr_dest[n - (1 + i)] = ptr_src[n - (1 + i)];
			i++;
		}
	}
	return (dest);
}
