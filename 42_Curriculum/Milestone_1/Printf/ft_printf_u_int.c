#include "ft_printf.h"

/**
 * @brief  Writes an unsigned decimal integer to stdout and updates the length counter.
 *
 * @details  Recursively divides `num` by 10 until the value falls below 10, then
 *           unwinds the call stack writing one decimal digit per frame in
 *           most-significant to least-significant order. Each frame increments `len`
 *           by 1 and writes directly via `write(2)` on fd 1.
 */
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
