#include "libft.h"

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
