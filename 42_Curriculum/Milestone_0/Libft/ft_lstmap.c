#include "libft.h"

/*
** Applies a function to each node's content and builds a new linked list
** from the results.
**
** Parameters:
** lst : pointer to the first node of the source list
** f   : function applied to each node's content; returns the new content
** del : function used to free a node's content on allocation failure
**
** Returns:
** A pointer to the first node of the newly created list.
** NULL if lst, f, or del is NULL, or if any allocation fails.
**
** Behavior:
** - Iterates over each node of lst from head to tail
** - Calls f(lst->content) and passes the result to ft_lstnew
** - Appends each new node to the result list via ft_lstadd_back
** - If ft_lstnew fails, calls ft_lstclear on the partial result list
**   using del, then returns NULL
**
** Note:
** The original list is never modified.
** f is responsible for allocating the new content if needed.
** del must be able to free whatever f returns, as it is passed directly
** to ft_lstclear on failure.
** If f returns a pointer to unmanaged memory (e.g. static or stack),
** del must handle it without calling free.
*/
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*node;
	t_list	*new_lst;

	if (!lst || !f)
		return (NULL);
	new_lst = NULL;
	while (lst != NULL)
	{
		node = ft_lstnew((*f)(lst->content));
		if (node == NULL)
		{
			ft_lstclear(&new_lst, (*del));
			return (NULL);
		}
		ft_lstadd_back(&new_lst, node);
		lst = lst->next;
	}
	return (new_lst);
}
