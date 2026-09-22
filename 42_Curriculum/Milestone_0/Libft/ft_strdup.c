#include "libft.h"

/*
** Creates a newly allocated copy of the string 's'.
**
** Parameters:
** s : null-terminated string to duplicate
**
** Returns:
** A pointer to the newly allocated duplicate string.
** NULL if memory allocation fails.
*/
char	*ft_strdup(const char *s)
{
	char	*str_dup;
	int		s_len;
	int		i;

	s_len = ft_strlen(s);
	str_dup = (char *)malloc(sizeof(char) * (s_len + 1));
	if (str_dup == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		str_dup[i] = s[i];
		i++;
	}
	str_dup[i] = '\0';
	return (str_dup);
}
