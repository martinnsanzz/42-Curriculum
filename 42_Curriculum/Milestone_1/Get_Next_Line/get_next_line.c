#include "get_next_line.h"

/*
** Reads and returns the next line from a file descriptor.
**
** Parameters:
** fd : file descriptor to read from
**
** Returns:
** A newly allocated null-terminated string containing the next line,
** '\n' included if present.
** NULL on allocation failure, read error, or EOF with no remaining data.
**
** Behavior:
** - Returns NULL if fd < 0
** - Initializes the static buffer to an empty string on first call
** - Calls fill_buffer() to append data until '\n' is found or EOF
** - Calls extract_line() to isolate and allocate the next line
** - Calls trim_buffer() to remove the extracted line from the buffer,
**   preserving any leftover bytes for the next call
** - Frees the old buffer pointer after trimming
**
** Note:
** Uses a static buffer that persists across calls for the same fd.
** Mixing multiple file descriptors with a single static buffer produces
** undefined behavior — each fd stomps the shared state.
*/
char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*result;
	char		*tmp;

	result = NULL;
	if (fd < 0)
		return (NULL);
	if (buffer == NULL)
	{
		buffer = ft_calloc(1, sizeof(char));
		if (buffer == NULL)
			return (NULL);
	}
	if (!fill_buffer(&buffer, fd))
	{
		free(buffer);
		return (NULL);
	}
	result = extract_line(buffer);
	tmp = buffer;
	buffer = trim_buffer(tmp);
	free(tmp);
	return (result);
}

/*
** Extracts the next line from the buffer, including the newline character.
**
** Parameters:
** buffer : null-terminated string containing buffered file content
**
** Returns:
** A newly allocated null-terminated string containing the line.
** NULL if buffer is NULL, empty, or allocation fails.
**
** Behavior:
** - If a '\n' is found, copies only up to (including) the newline
** - If no '\n' is found, copies the entire buffer content
** - Allocates len + 1 bytes (ft_calloc zeroes the buffer, null-term is implicit)
**
** Note:
** Caller is responsible for freeing the returned string.
*/
char	*extract_line(char	*buffer)
{
	char	*line;
	char	*new_line;
	size_t	len;

	if (buffer == NULL || buffer[0] == '\0')
		return (NULL);
	line = ft_strchr(buffer, '\n');
	if (line == NULL)
		len = ft_strlen(buffer);
	else
		len = line - buffer + 1;
	new_line = ft_calloc(len + 1, sizeof(char));
	if (new_line == NULL)
		return (NULL);
	ft_strncpy(new_line, buffer, len);
	return (new_line);
}

/*
** Allocates and returns a new string containing everything after
** the first '\n' in buffer.
**
** Parameters:
** buffer : null-terminated string containing buffered file content
**
** Returns:
** New allocated string with remaining content after '\n'.
** NULL if buffer is NULL, empty, or no '\n' is found.
*/
char	*trim_buffer(char *buffer)
{
	char	*new_buffer;
	char	*tmp;

	if (!buffer || buffer[0] == '\0' || !ft_strchr(buffer, '\n'))
		return (NULL);
	tmp = ft_strchr(buffer, '\n') + 1;
	new_buffer = ft_calloc(ft_strlen(tmp) + 1, sizeof(char));
	if (new_buffer == NULL)
		return (NULL);
	new_buffer = ft_strncpy(new_buffer, tmp, ft_strlen(tmp));
	return (new_buffer);
}

/*
** Reads from a file descriptor into a buffer until a newline is found 
** or EOF is reached.
**
** Parameters:
** buffer : pointer to a heap-allocated null-terminated string accumulating 
** 			file content
** fd     : file descriptor to read from
**
** Returns:
** 1 on success (newline found or EOF reached).
** 0 on allocation failure or read error.
**
** Behavior:
** - Initializes *buffer to an empty string if NULL (required for ft_strjoin).
** - Reads up to BUFFER_SIZE bytes per iteration into a temporary chunk.
** - Appends each chunk to *buffer via ft_strjoin; frees the previous *buffer.
** - Stops when '\n' is found in *buffer or bytes read reaches 0 (EOF).
** - Clears tmp_buffer with ft_bzero before each read to avoid stale data.
**
** Note:
** If ft_strjoin fails mid-loop, *buffer becomes NULL and the previous buffer
** is freed, but tmp_buffer is NOT freed → memory leak.
** If *buffer allocation fails after tmp_buffer is allocated → tmp_buffer leaks.
** Caller is responsible for freeing *buffer on success.
*/
int	fill_buffer(char **buffer, int fd)
{
	char	*tmp;
	char	*tmp_buffer;
	ssize_t	bytes;

	bytes = 1;
	tmp = NULL;
	tmp_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (tmp_buffer == NULL)
		return (0);
	while (!ft_strchr(*buffer, '\n') && bytes > 0)
	{
		tmp = *buffer;
		ft_bzero(tmp_buffer, BUFFER_SIZE);
		bytes = read(fd, tmp_buffer, BUFFER_SIZE);
		*buffer = ft_strjoin(tmp, tmp_buffer);
		if (*buffer == NULL)
			free(tmp_buffer);
		free(tmp);
	}
	free (tmp_buffer);
	return (1);
}

/*
** Allocates and returns a new string with 's1' as prefix and
** 's2' as suffix
**
** Parameters:
** s1: The prefix string.
** s2: The suffix string.
**
** Returns:
** The new string.
** NULL if the allocation fails.
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		total_len;
	int		i;

	if (!s1)
		s1 = "";
	total_len = ft_strlen(s1) + ft_strlen(s2);
	str = ft_calloc(total_len + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (*s1 != '\0')
		str[i++] = *s1++;
	while (*s2 != '\0')
		str[i++] = *s2++;
	return (str);
}

// #include <fcntl.h>
// #include <stdio.h>

// int	main(void)
// {
// 	int		fd;
// 	char	*line;

// 	fd = open("test.txt", O_RDONLY);
// 	//fd = 0;
// 	while (1)
// 	{
// 		line = get_next_line(fd);
// 		if (line == NULL)
// 			break ;
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }
