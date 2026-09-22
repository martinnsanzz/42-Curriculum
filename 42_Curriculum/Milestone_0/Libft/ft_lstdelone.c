#include "libft.h"

/*
** Deletes a single linked list node and frees its memory.
**
** Parameters:
** lst : pointer to the node to delete
** del : function pointer used to free the node's content
**
** Returns:
** void
**
** Behavior:
** - Calls del on lst->content to free the content
** - Calls free on the node itself
** - Does NOT unlink the node from the list (caller's responsibility)
** - Returns early if lst is NULL
**
** Note:
** del must not be NULL (undefined behavior if NULL is passed).
** Does not affect neighboring nodes — caller must update surrounding
** pointers before calling this function to avoid dangling pointers.
*/
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL || (*del) == NULL)
		return ;
	(*del)(lst->content);
	free(lst);
}
