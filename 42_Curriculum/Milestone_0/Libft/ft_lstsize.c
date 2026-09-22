#include "libft.h"

/*
** Counts the number of nodes in a linked list.
**
** Parameter:
** lst : pointer to the first node of the list
**
** Returns:
** The total number of nodes in the list.
*/
int	ft_lstsize(t_list *lst)
{
	int		size;

	size = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}
