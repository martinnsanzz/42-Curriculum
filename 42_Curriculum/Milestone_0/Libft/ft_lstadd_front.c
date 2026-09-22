#include "libft.h"

/*
** Adds the node 'new' at the beginning of the list.
**
** Parameters:
** lst : pointer to the head pointer of the list
** new : node to insert at the front of the list
**
** Behavior:
** The new node becomes the first element of the list.
** Its next pointer is set to the previous head.
*/
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}
