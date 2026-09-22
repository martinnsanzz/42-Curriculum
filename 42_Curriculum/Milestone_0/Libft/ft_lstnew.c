#include "libft.h"

/*
** Allocates and initializes a new list node.
**
** Parameters:
** content : pointer to the data to store in the new node
**
** Returns:
** - A pointer to the newly created node.
** - NULL if memory allocation fails.
**
** Notes:
** The content pointer is stored directly and not duplicated.
** The next pointer is initialized to NULL.
*/
t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (node == NULL)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
