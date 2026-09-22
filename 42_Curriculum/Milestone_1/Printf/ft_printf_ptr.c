#include "ft_printf.h"
#include <stdint.h>

/**
 * @brief  Writes a pointer address to stdout in hex format and updates the length counter.
 *
 * @details  If `p` is NULL, prints the string "(nil)" to fd 1 and increments `len`
 *           accordingly. For non-NULL pointers, casts `p` to `uintptr_t` and prints the
 *           address prefixed with "0x" in lowercase hexadecimal via `ft_putnbr_base`.
 *
 * @note  `ft_putnbr_base` is responsible for both printing the hex digits and
 *        updating `len` for that portion of the output.
 */
void	ft_printf_ptr(void *p, int *len)
{
	uintptr_t	ptr;
	const char	*lower_base;

	if (!len)
		return ;
	lower_base = "0123456789abcdef";
	if (p == NULL)
	{
		ft_putstr_fd("(nil)", 1);
		*len += ft_strlen("(nil)");
		return ;
	}
	ptr = (uintptr_t)p;
	ft_putstr_fd("0x", 1);
	*len += 2;
	ft_putnbr_base(ptr, lower_base, len);
}
