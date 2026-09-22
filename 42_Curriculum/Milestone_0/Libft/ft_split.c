#include "libft.h"

static void	ft_free(char **arr, int word);
static int		ft_word_count(char	*s, char c);
static int		ft_word_len(char *s, char c);

/*
** Splits the string s into an array of strings using the delimiter c.
**
** Parameters:
** s : null-terminated string to split
** c : delimiter character
**
** Returns:
** A NULL-terminated array of strings (words).
** NULL if memory allocation fails.
**
** Behavior:
** - Consecutive delimiters are ignored
** - Each word is allocated separately
** - The resulting array is also dynamically allocated
**
** Note:
** On allocation failure, all previously allocated memory is freed.
*/
char	**ft_split(char const *s, char c)
{
	char	**arr;
	char	*str;
	size_t	word;

	str = (char *)s;
	if (str == NULL)
		return (NULL);
	arr = ft_calloc(ft_word_count(str, c) + 1, sizeof(char *));
	if (arr == NULL)
		return (NULL);
	word = 0;
	while (*str != '\0')
	{
		while (*str == c)
			str++;
		if (*str != '\0')
		{
			arr[word] = ft_substr(str, 0, ft_word_len(str, c));
			if (!arr[word++])
				return (ft_free(arr, word - 1), NULL);
			str += ft_word_len(str, c);
		}
	}
	arr[word] = NULL;
	return (arr);
}

/*
** Counts the number of words in a null-terminated string delimited by c.
**
** Parameters:
** s : input null-terminated string
** c : delimiter character
**
** Returns:
** Number of words found.
**
** Behavior:
** - A word is a sequence of characters not equal to c.
** - Detects word start on transition: previous char == c (or i == 0).
** - Consecutive delimiters are skipped (not counted as empty words).
*/
static int	ft_word_count(char	*s, char c)
{
	int	word_count;
	int	len;
	int	i;

	len = ft_strlen(s);
	word_count = 0;
	i = 0;
	while (i < len)
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			word_count++;
		i++;
	}
	return (word_count);
}

/*
** Computes the length of a word starting at s,
** stopping at delimiter c or end of string.
*/
static int	ft_word_len(char *s, char c)
{
	int	word_len;

	word_len = 0;
	while (s[word_len] != '\0')
	{
		if (s[word_len] == c)
			return (word_len);
		word_len++;
	}
	return (word_len);
}

/*
** Frees a partially allocated array of strings.
**
** Parameters:
** arr  : array of strings
** word : number of successfully allocated strings
**
** Behavior:
** Frees all allocated strings and the array itself.
*/
static void	ft_free(char **arr, int word)
{
	while (word > 0)
	{
		word--;
		free(arr[word]);
	}
	free(arr);
}
