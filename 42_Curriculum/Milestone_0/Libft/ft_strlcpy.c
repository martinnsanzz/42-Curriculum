#include "libft.h"

/*
** Copies the string src into dst, writing at most dstsize bytes.
**
** The function copies up to dstsize - 1 characters from src into dst
** and always null-terminates dst when dstsize > 0.
**
** Parameters:
** dst      : destination buffer where src will be copied
** src      : null-terminated source string
** dstsize  : total size of the destination buffer
**
** Returns:
** The length of src (number of characters excluding '\0').
**
** Note:
** If the returned value is >= dstsize, truncation occurred.
*/
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize >= 1)
	{
		while (i < (dstsize - 1) && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}
