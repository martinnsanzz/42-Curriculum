#include "libft.h"

static void	ft_handle_spec(char c, va_list *args, int *len);
static int	check_format(char c);

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		len;

	if (format == NULL)
		return (-1);
	va_start(args, format);
	len = 0;
	while (*format != '\0')
	{
		if (*format == '%')
		{
			if (!check_format(*(format + 1)))
				break ;
			else
				ft_handle_spec(*(format + 1), &args, &len);
			format++;
		}
		else
			ft_printf_char(*format, &len);
		format++;
	}
	va_end(args);
	return (len);
}

static void	ft_handle_spec(char c, va_list *args, int *len)
{
	if (!len)
		return ;
	if (c == '%')
		ft_printf_char('%', len);
	else if (c == 'c')
		ft_printf_char(va_arg(*args, int), len);
	else if (c == 'x' || c == 'X')
		ft_printf_hex(va_arg(*args, unsigned int), c, len);
	else if (c == 'd' || c == 'i')
		ft_printf_int(va_arg(*args, int), len);
	else if (c == 'u')
		ft_printf_u_int(va_arg(*args, unsigned int), len);
	else if (c == 'p')
		ft_printf_ptr(va_arg(*args, void *), len);
	else if (c == 's')
		ft_printf_str(va_arg(*args, char *), len);
}

static int	check_format(char c)
{
	c = (unsigned char)c;
	if (c == '%'
		|| c == 'c'
		|| c == 'x'
		|| c == 'X'
		|| c == 'd'
		|| c == 'u'
		|| c == 'i'
		|| c == 'p'
		|| c == 's')
		return (1);
	ft_putstr_fd("[ERROR: Incorrect format specifier - '", 2);
	write(2, &c, 1);
	ft_putstr_fd("']", 2);
	return (0);
}
