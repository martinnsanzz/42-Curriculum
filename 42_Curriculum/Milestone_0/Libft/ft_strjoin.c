#include "libft.h"

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
