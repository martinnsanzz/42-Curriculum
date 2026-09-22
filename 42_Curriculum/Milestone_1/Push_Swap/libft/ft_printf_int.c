#include "libft.h"

void	ft_printf_int(int num, int *len)
{
	char	*s_num;

	if (!len)
		return ;
	s_num = ft_itoa(num);
	if (!s_num)
		return ;
	*len += ft_strlen(s_num);
	ft_putnbr_fd(num, 1);
	free(s_num);
}
