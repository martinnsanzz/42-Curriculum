#include "libft.h"

/*
** Writes a single character to a file descriptor.
**
** Parameters:
** c  : character to write
** fd : file descriptor to write to
**
** Returns:
** Nothing (void).
**
** Behavior:
** - Writes exactly 1 byte to 'fd' using write(2)
**
** Note:
** No validation on 'fd' — passing an invalid or closed fd causes
** write(2) to fail silently (return value is ignored).
*/
void	ft_putchar_fd(char c, int fd)
{
	if (c == '\0')
		return ;
	write(fd, &c, 1);
}
