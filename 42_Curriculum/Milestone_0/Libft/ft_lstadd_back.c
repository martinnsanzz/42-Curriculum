#include "libft.h"

/*
** Adds the node 'new' at the end of the list.
**
** Parameters:
** lst : pointer to the head pointer of the list
** new : node to append at the end of the list
**
** Behavior:
** - If the list is empty, 'new' becomes the first node
** - Otherwise, 'new' is added after the last node
** - The next pointer of 'new' is set to NULL
**
** Note:
** The function uses ft_lstlast to find the last node of the list.
*/
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last_node;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_node = ft_lstlast(*lst);
	last_node->next = new;
}
