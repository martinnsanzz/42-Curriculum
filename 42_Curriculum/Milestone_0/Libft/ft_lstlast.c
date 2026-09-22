#include "libft.h"

/*
** Returns the last node of a linked list.
**
** Parameters:
** lst : pointer to the first node of the list
**
** Returns:
** A pointer to the last node of the list.
** NULL if the list is empty.
*/
t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current;

	if (lst == NULL)
		return (NULL);
	current = lst;
	while (current->next != NULL)
		current = current->next;
	return (current);
}
