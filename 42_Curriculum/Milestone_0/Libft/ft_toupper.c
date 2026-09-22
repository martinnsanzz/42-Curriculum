#include "libft.h"

/*
** Converts a lowercase ASCII letter to it uppercase equivelant.
**
** Parameter:
**	c: char value to convert
**
** Return:
** The uppercase version of the character if it is a lowercase letter.
** Otherwise, the input value is returned unchanged.

** Notes:
** The input is cast to unsigned char to ensure correct behavior
** when c is outside the standard ASCII range.
*/
int	ft_toupper(int c)
{
	c = (unsigned char)c;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
