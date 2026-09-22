#include "libft.h"

/*
** Locates the last occurrence of character c in the null-terminated string s.
**
** Parameters:
** s : pointer to the null-terminated string to search
** c : character to locate, passed as int and cast to char
**
** Returns:
** A pointer to the last occurrence of c in s.
** A pointer to the null terminator if c is '\0'.
** NULL if c is not found.
**
** Note:
** Passing NULL for s is undefined behavior.
** The returned pointer is non-const; modifying s through it is the
** caller's responsibility.
*/
char	*ft_strrchr(const char *s, int c)
{
	int		len;
	char	*str;

	c = (char)c;
	str = (char *)s;
	len = ft_strlen(str);
	if (c == str[len])
		return (&str[len]);
	while (len >= 0)
	{
		if (c == str[len])
			return (&str[len]);
		len--;
	}
	return (NULL);
}
