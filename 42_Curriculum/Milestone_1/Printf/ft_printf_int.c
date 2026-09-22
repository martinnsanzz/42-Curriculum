#include "ft_printf.h"

/**
 * @brief  Writes a decimal integer to stdout and adds its digit count to the length counter.
 *
 * @details  Converts `num` to its string representation via `ft_itoa` solely to
 *           determine the printed length. The actual output is performed by
 *           `ft_putnbr_fd` on fd 1.
 * 
 * @note  Relies on `ft_itoa` for length measurement and `ft_putnbr_fd` for output.
 */
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
