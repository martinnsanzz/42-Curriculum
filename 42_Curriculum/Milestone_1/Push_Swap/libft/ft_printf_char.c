#include "libft.h"

void	ft_printf_char(int c, int *len)
{
	if (!len)
		return ;
	c = (unsigned char)c;
	if (c == '\0')
		write(1, "\0", 1);
	else
		ft_putchar_fd(c, 1);
	*len += 1;
}
