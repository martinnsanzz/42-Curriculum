#include "ft_printf.h"

/**
 * @brief  Writes a string to stdout and adds its length to the output length counter.
 *
 * @details  If `s` is NULL, prints the literal "(null)" to fd 1 and increments `len`
 *           by 6, then returns early. For valid strings, computes the length via
 *           `ft_strlen` before printing so that `len` is updated even if the string
 *           is empty. Output is performed by `ft_putstr_fd` on fd 1.
 */
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
