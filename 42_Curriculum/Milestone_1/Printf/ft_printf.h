#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdarg.h> //This defines the macros va_start, va_arg, va_end, va_copy
# include "libft/libft.h"

int		ft_printf(const char *format, ...);
void	ft_printf_hex(unsigned long num, char conv, int *len);
void	ft_printf_int(int num, int *len);
void	ft_printf_u_int(unsigned int num, int *len);
void	ft_printf_ptr(void *p, int *len);
void	ft_printf_str(char *s, int *len);
void	ft_printf_char(int c, int *len);
void	ft_putnbr_base(unsigned long num, const char *base, int *len);

#endif