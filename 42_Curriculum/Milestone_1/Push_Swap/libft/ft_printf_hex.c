#include "libft.h"

void	ft_printf_hex(unsigned long num, char conv, int *len)
{
	const char	*lower_base;
	const char	*upper_base;

	if (!len)
		return ;
	lower_base = "0123456789abcdef";
	upper_base = "0123456789ABCDEF";
	if (conv == 'x')
		ft_putnbr_base(num, lower_base, len);
	else if (conv == 'X')
		ft_putnbr_base(num, upper_base, len);
}
