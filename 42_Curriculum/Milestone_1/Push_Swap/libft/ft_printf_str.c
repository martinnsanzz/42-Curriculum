#include "libft.h"

void	ft_printf_str(char *s, int *len)
{
	if (!len)
		return ;
	if (s == NULL)
	{
		ft_putstr_fd("(null)", 1);
		*len += ft_strlen("(null)");
		return ;
	}
	*len += ft_strlen(s);
	ft_putstr_fd(s, 1);
}
