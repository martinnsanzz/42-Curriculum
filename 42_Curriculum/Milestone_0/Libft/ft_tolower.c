#include "libft.h"

/*
** Converts a uppercase ASCII letter to it lowercase equivelant.
**
** Parameter:
**	c: char value to convert
**
** Return:
** The lowercase version of the character if it is a uppercase letter.
** Otherwise, the input value is returned unchanged.

** Notes:
** The input is cast to unsigned char to ensure correct behavior
** when c is outside the standard ASCII range.
*/
int	ft_tolower(int c)
{
	c = (unsigned char)c;
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}
