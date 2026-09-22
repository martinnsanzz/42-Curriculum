#include "libft.h"

void	ft_printf_u_int(unsigned int num, int *len)
{
	char			c;

	if (!len)
		return ;
	if (num >= 10)
	{
		ft_printf_u_int(num / 10, len);
	}
	c = (num % 10) + '0';
	write (1, &c, 1);
	*len += 1;
}
