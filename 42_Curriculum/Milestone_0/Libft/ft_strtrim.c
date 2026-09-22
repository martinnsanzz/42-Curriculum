#include "libft.h"

/*
** Allocates and returns a trimmed version of s1 removing 'set'
** characters from the beginning and the end
**
** Parameters:
** s1	: The string to be trimmed.
** set	: The string containing the set of characters
** 		  to be removed
**
** Returns:
** The trimmed string
** An empty string if 's1' is empty
** NULL if the allocation fails
**
** Behaviour:
** The index moves forward or backwards if set is
** found in the string with ft_strchr
*/
char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s1_trim;
	size_t	start;
	size_t	end;
	size_t	size;

	start = 0;
	end = ft_strlen(s1);
	if (end == 0)
		return (s1_trim = ft_calloc(1, sizeof(char)));
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	size = end - start;
	s1_trim = ft_calloc(size + 1, sizeof(char));
	if (s1_trim == NULL)
		return (NULL);
	s1_trim = ft_memcpy(s1_trim, &s1[start], size);
	return (s1_trim);
}
