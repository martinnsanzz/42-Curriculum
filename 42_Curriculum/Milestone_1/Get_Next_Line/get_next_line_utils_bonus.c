#include "get_next_line_bonus.h"

/*
** Copies n amount of character to dest from src
**
** Parameters:
** dest		: String to copy to
** src		: Null terminated string to copy from
** n		: Amount of characters to be copied
**
** Returns:
** Copied string
*/
char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	unsigned int	i;

	if (src == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

/*
** Allocates memory for an array of 'nmemb' elements of 'size' bytes each.
** The allocated memory block is initialized to zero.
**
** Parameters:
** nmemb : number of elements to allocate
** size  : size in bytes of each element
**
** Returns:
** A pointer to the allocated memory initialized to zero.
** NULL if the allocation fails or memory overflow is detected.
**
** Note:
** The memory is initialized to zero using ft_bzero.
*/
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}

/*
** Computes the length of a null-terminated string.
** Parameters:
** s	: null-terminated string to measure
**
** Returns:
** Number of characters in the string, excluding the terminating '\0'.
*/
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

/*
** Null terminates '\0' the first n bytes of the memory area pointed to by s
** 
** Parameters:
** s : pointer to the memory block to modify
** n : number of bytes to write
**
** Note:
** Memory is written byte-by-byte, therefore the pointer is cast
** to unsigned char* to allow single-byte access.
*/
void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = '\0';
		i++;
	}
}

/*
** Finds a character inside a string and returns a pointer to that location
**
** Parameters:
** c	: Character to find
** s	: Null terminate dstring to look at
**
** Returns:
** Pointer to the character found in the string
** Null if nothing is found
*/
char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str;

	c = (unsigned char)c;
	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (&str[i]);
		i++;
	}
	if (c == str[i])
		return (&str[i]);
	return (NULL);
}