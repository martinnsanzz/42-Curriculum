#include "libft.h"

/*
** Writes a null-terminated string to a file descriptor, byte by byte followed
** by a new line.
**
** Parameters:
** s  : null-terminated string to write
** fd : file descriptor to write to
**
** Returns:
** Nothing (void).
**
** Behavior:
** - Iterates through each character until '\0' is reached
** - Writes one byte at a time using write(2)
** - Stops at the null terminator (not written)
**
** Note:
** Undefined behavior if s is NULL.
** No error handling on write() failures.
** Caller is responsible for passing a valid, open file descriptor.
*/
void	ft_putendl_fd(char *s, int fd)
{
	if (s == NULL)
		return ;
	while (*s != '\0')
		write(fd, s++, 1);
	write(fd, "\n", 1);
}
