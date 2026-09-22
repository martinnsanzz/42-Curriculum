#include "libft.h"

/*
** Checks whether a character is an alphabetic letter.
*/
int	ft_isalpha(int c)
{
	c = (unsigned char)c;
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
