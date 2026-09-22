#include "libft.h"

/*
** Allocates and returns a substring from string s.
**
** The substring starts at index 'start' and has a maximum length of 'len'.
**
** Parameters:
** s     : original null-terminated string
** start : starting index of the substring
** len   : maximum length of the substring
**
** Returns:
** A newly allocated null-terminated substring.
** An empty string if start is beyond the end of s.
** NULL if memory allocation fails.
**
** Note:
** The length of the substring is adjusted if it exceeds
** the remaining characters in s.
*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	sub_len;
	char	*sub;
	int		i;

	if (ft_strlen(s) < (size_t)start)
		return (sub = ft_calloc(1, sizeof(char)));
	sub_len = ft_strlen(s + start);
	if (len > sub_len)
		len = sub_len;
	if (sub_len > 0 || len == 0)
		sub = ft_calloc(len + 1, sizeof(char));
	i = 0;
	while (s[start + i] && len--)
	{
		sub[i] = s[start + i];
		i++;
	}
	ft_bzero(&sub[i], 1);
	return (sub);
}
