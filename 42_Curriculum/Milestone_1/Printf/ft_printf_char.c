#include "ft_printf.h"

/**
 * @brief  Writes a single character to stdout and increments the output length counter.
 *
 * @details  Casts the integer `c` to `unsigned char` to normalize any sign-extended
 *           values before output. If the character is the null terminator '\0', it is
 *           written explicitly using `write(2)`. All other characters are forwarded 
 * 			 to `ft_putchar_fd` on fd 1. The length counter is unconditionally incremented
 * 			 by 1 after output.
 */
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
