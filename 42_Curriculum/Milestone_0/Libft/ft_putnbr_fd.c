#include "libft.h"

/*
** Outputs an integer 'n' to the specified file descriptor.
**
** Parameters:
** n	: Integer to output
** fb	: File descriptor to write too
**
** Behavior:
** - Converts number to long to deal with INT_MIN
** - Checks if n is negative and flips the number and outputs -
** - Calls the function over until n is smaller than 10 to separete the digits
**
** Returns:
** Nothing (void) function.
*/
void	ft_putnbr_fd(int n, int fd)
{
	long	nb;
	char	c;

	nb = n;
	if (nb < 0)
	{
		write(fd, "-", 1);
		nb *= -1;
	}
	if (nb >= 10)
		ft_putnbr_fd(nb / 10, fd);
	c = (nb % 10) + '0';
	write (fd, &c, 1);
}
