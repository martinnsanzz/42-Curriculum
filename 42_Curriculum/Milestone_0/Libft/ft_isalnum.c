#include "libft.h"

/*
** Checks whether a character is an alphanumeric character.
** An alphanumeric character is either a letter (A-Z, a-z)
** or a digit (0-9).
*/
int	ft_isalnum(int c)
{
	c = (unsigned char)c;
	return (ft_isalpha(c) || ft_isdigit(c));
}
