#include "libft.h"

/*
** Checks whether a character is a decimal digit (0-9).
*/
int	ft_isdigit(int c)
{
	c = (unsigned char)c;
	return (c >= '0' && c <= '9');
}
