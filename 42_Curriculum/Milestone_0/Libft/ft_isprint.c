#include "libft.h"

/*
** Checks whether a character is printable.
** Printable ASCII range: 32 (space) to 126 (~).
*/
int	ft_isprint(int c)
{
	c = (unsigned char)c;
	return (c >= ' ' && c <= '~');
}
