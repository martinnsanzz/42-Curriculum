#include "libft.h"

/*
** Checks whether a value belongs to the ASCII table.
*/
int	ft_isascii(int c)
{
	if (c > 255)
		return (0);
	c = (unsigned char)c;
	return (c >= 0 && c <= 127);
}
