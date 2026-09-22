#include "libft.h"

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
