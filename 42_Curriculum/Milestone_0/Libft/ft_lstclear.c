#include "libft.h"

/*
** Frees all nodes of a linked list using a deletion function.
**
** Parameters:
** lst : address of a pointer to the first node of the list
** del : function pointer used to free the content of each node
**
** Returns:
** Nothing (void).
**
** Behavior:
** - Iterates through every node of the list
** - Calls del() on each node's content before freeing the node
** - Saves the next pointer before freeing to avoid use-after-free
** - Sets *lst to NULL after clearing to prevent dangling pointer
** - Returns early if lst or del is NULL
**
** Note:
** Does not free *lst itself as a pointer — only the nodes it points to.
** The caller is responsible for ensuring del correctly frees the content.
** After this call, *lst is NULL and the list is fully deallocated.
*/
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current;
	t_list	*next;

	if (lst == NULL || del == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		(*del)(current->content);
		free(current);
		current = next;
	}
	*lst = NULL;
}
