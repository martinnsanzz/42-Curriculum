#include "libft.h"

static int	check_base_error(const char *base);
static void	convert_base(unsigned long num, const char *base,
				unsigned long base_size, int *len);

void	ft_putnbr_base(unsigned long num, const char *base, int *len)
{
	unsigned long	base_size;

	if (!len || !base || check_base_error(&base[0]))
		return ;
	base_size = 0;
	while (base[base_size] != '\0')
		base_size++;
	convert_base(num, &base[0], base_size, len);
}

static void	convert_base(unsigned long num, const char *base,
	unsigned long base_size, int *len)
{
	if (num >= (unsigned long)base_size)
	{
		convert_base(num / base_size, &base[0], base_size, len);
		num = num % base_size;
	}
	ft_putchar_fd(base[num % base_size], 1);
	*len += 1;
}

static int	check_base_error(const char *base)
{
	int	j;
	int	i;

	if (!base)
		return (1);
	i = 0;
	while (base[i] != '\0')
	{
		if (base[i] == ' ' || (base[i] >= 9 && base[i] <= 13) || base[i] == '+'
			|| base[i] == '-')
			return (1);
		j = i + 1;
		while (base[j] != '\0')
		{
			if (base[i] == base[j])
				return (1);
			j++;
		}
		i++;
	}
	if (i < 2)
		return (1);
	return (0);
}
