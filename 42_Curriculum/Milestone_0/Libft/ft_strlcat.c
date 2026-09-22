#include "libft.h"

/*
** Appends src to dst, ensuring the result is null-terminated and at most
** dstsize bytes total (including the null terminator).
**
** Parameters:
** dst     : pointer to the null-terminated destination string
** src     : pointer to the null-terminated source string
** dstsize : total size of the destination buffer in bytes
**
** Returns:
** The total length that the concatenated string would have if dstsize were
** large enough: dst_len + src_len.
** If dst_len >= dstsize, returns dstsize + src_len (truncation indicator).
**
** Behavior:
** - Computes dst_len and src_len via ft_strlen before any modification
** - If dst_len >= dstsize, no bytes are written; returns dstsize + src_len
** - Otherwise, appends at most (dstsize - dst_len - 1) bytes from src
** - Always null-terminates dst after appending
**
** Note:
** dst must be a valid null-terminated string of length < dstsize.
** If dst is not null-terminated within dstsize bytes, ft_strlen behavior
** is undefined.
** The return value > dstsize indicates output was truncated.
*/
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = 0;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	if (dstsize >= 1)
	{
		while (i < (dstsize - dst_len - 1) && src[i])
		{
			dst[dst_len + i] = src[i];
			i++;
		}
		dst[dst_len + i] = '\0';
	}
	return (dst_len + src_len);
}
