#include "ft_printf.h"

static void	ft_handle_spec(char c, va_list *args, int *len);
static int	check_format(char c);

/**
 * @brief  Produces formatted output to stdout according to a format string.
 *
 * @details  Iterates through each character of `format`. Plain characters are printed
 *           directly via `ft_printf_char`. A '%' triggers format specifier validation
 *           via `check_format` on the following character — if invalid, iteration stops
 *           immediately. If valid, `ft_handle_spec` dispatches the specifier and the
 *           format pointer is advanced past it. The total number of characters written
 *           is tracked in `len` and returned on success.
 *
 * @param  format  const char* — The format string containing literal characters and
 *                               '%'-prefixed specifiers. Supported specifiers: '%', 'c',
 *                               's', 'd', 'i', 'u', 'x', 'X', 'p'. Must not be NULL.
 * @param  ...     — Variadic arguments matching each format specifier in order.
 *                   Type mismatches cause undefined behavior.
 *
 * @return int — Total number of characters written to stdout on success.
 *               Returns -1 if `format` is NULL.
 *               Returns a partial count if an invalid specifier is encountered
 *               (iteration breaks early).
 */
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

/**
 * @brief  Dispatches a validated format specifier to its corresponding print handler.
 *
 * @details  Acts as the central routing function for ft_printf format specifiers.
 *           Each branch extracts the appropriate argument from `args` via `va_arg`
 *           and forwards it to the matching handler. The '%%' case prints a literal
 *           '%' without consuming a variadic argument. All handlers are responsible
 *           for updating `len` directly.
 *
 * @param  c     char      — The validated format specifier character. Expected to be
 *                           one of: '%', 'c', 'x', 'X', 'd', 'i', 'u', 'p', 's'.
 *                           Passing an unrecognized character is a no-op.
 * @param  args  va_list   — The active variadic argument list. Must be initialized
 *                           by the caller via `va_start` before this call. Each
 *                           branch consumes exactly one argument, except '%'.
 * @param  len   int*      — Pointer to the running output length counter. Passed
 *                           through to each handler for direct update. Must not be NULL.
 */
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

/**
 * @brief  Validates a printf format specifier character and reports errors to stderr.
 *
 * @details  Checks whether `c` matches one of the supported format specifiers:
 *           '%', 'c', 'x', 'X', 'd', 'u', 'i', 'p', or 's'. If valid, returns 1
 *           immediately. If invalid, writes an error message to fd 2
 *           (stderr) including the offending character, then returns 0.
 *
 * @return int — 1 if `c` is a supported format specifier, 0 otherwise.
 */
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
