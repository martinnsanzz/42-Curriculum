#include "libft.h"

/*
** Applies a function to each character of a string, building a new string.
**
** Parameters:
** s : input null-terminated string
** f : function pointer called with (index, char) for each character
**
** Returns:
** A newly allocated null-terminated string with transformed characters.
** NULL if 's' is NULL or if allocation fails.
**
** Behavior:
** - Iterates over each character of 's' with its unsigned int index
** - Writes the result of f(i, s[i]) into the output string
** - The output string is null-terminated via ft_calloc zero-initialization
**
** Note:
** The caller is responsible for freeing the returned string.
** Behavior is undefined if 'f' is NULL.
*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	if (s == NULL)
		return (NULL);
	str = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		str[i] = (*f)(i, s[i]);
		i++;
	}
	return (str);
}
