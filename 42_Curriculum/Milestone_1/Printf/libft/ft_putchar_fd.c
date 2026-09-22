#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	if (c == '\0')
		return ;
	write(fd, &c, 1);
}
