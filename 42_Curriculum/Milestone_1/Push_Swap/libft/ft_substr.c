#include "libft.h"

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
