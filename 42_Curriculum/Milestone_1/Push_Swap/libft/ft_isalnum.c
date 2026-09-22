#include "libft.h"

int	ft_isalnum(int c)
{
	c = (unsigned char)c;
	return (ft_isalpha(c) || ft_isdigit(c));
}
