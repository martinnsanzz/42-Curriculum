#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

/* ---- Minimal t_list (remove if using libft.h) --------------------------- */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/* ---- Functions under test + required libft deps ------------------------- */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
int		ft_lstsize(t_list *lst);

/* ========================================================================= */
/*   HELPERS                                                                  */
/* ========================================================================= */

static t_list	*make_node(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

static t_list	*make_int_list(int *vals, int len)
{
	t_list	*head;
	t_list	*node;
	t_list	*cur;
	int		i;

	head = NULL;
	i = 0;
	while (i < len)
	{
		node = make_node(&vals[i]);
		if (!head)
			head = node;
		else
		{
			cur = head;
			while (cur->next)
				cur = cur->next;
			cur->next = node;
		}
		i++;
	}
	return (head);
}

static void	free_list(t_list *lst)
{
	t_list	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}

static void	del_free(void *content)
{
	free(content);
}

static void	del_noop(void *content)
{
	(void)content;
}

/* ---- Tracking ------------------------------------------------------------ */
static int	g_del_count = 0;

static void	del_count(void *content)
{
	(void)content;
	g_del_count++;
}

static int	g_f_count = 0;

/* ---- Transform functions ------------------------------------------------- */

static void	*f_increment(void *content)
{
	int	*n;
	int	*result;

	n = (int *)content;
	result = malloc(sizeof(int));
	if (!result)
		return (NULL);
	*result = *n + 1;
	g_f_count++;
	return (result);
}

static void	*f_uppercase(void *content)
{
	char	*src;
	char	*dst;

	src = (char *)content;
	dst = malloc(strlen(src) + 1);
	if (!dst)
		return (NULL);
	strcpy(dst, src);
	if (dst[0] >= 'a' && dst[0] <= 'z')
		dst[0] -= 32;
	return (dst);
}

static void	*f_returns_null(void *content)
{
	(void)content;
	return (NULL);
}

static void	*f_identity(void *content)
{
	return (content);
}

/* ========================================================================= */
/*   TESTS                                                                    */
/* ========================================================================= */

/* --- GROUP 1: NULL guards ------------------------------------------------- */
static void	test_null_guards(void)
{
	t_list	*lst;
	t_list	*result;
	int		val;

	printf("=== GROUP 1: NULL guards ===\n");

	val = 1;
	lst = make_node(&val);

	result = ft_lstmap(NULL, f_increment, del_free);
	printf("[1.1] lst=NULL       | expected=NULL, got=%s | %s\n",
		result ? "non-NULL" : "NULL",
		result == NULL ? "PASS" : "FAIL");

	result = ft_lstmap(lst, NULL, del_free);
	printf("[1.2] f=NULL         | expected=NULL, got=%s | %s\n",
		result ? "non-NULL" : "NULL",
		result == NULL ? "PASS" : "FAIL");

	result = ft_lstmap(lst, f_increment, NULL);
	if (result)
		ft_lstclear(&result, del_free);
	printf("[1.3] del=NULL       | expected=NULL, got=%s | %s\n",
		result ? "non-NULL" : "NULL",
		result == NULL ? "PASS" : "FAIL");

	result = ft_lstmap(NULL, NULL, NULL);
	printf("[1.4] all NULL       | expected=NULL, got=%s | %s\n",
		result ? "non-NULL" : "NULL",
		result == NULL ? "PASS" : "FAIL");

	free(lst);
}

/* --- GROUP 2: original list is NOT modified ------------------------------- */
static void	test_original_untouched(void)
{
	int		vals[3];
	t_list	*lst;
	t_list	*result;

	printf("\n=== GROUP 2: original list untouched ===\n");

	vals[0] = 10; vals[1] = 20; vals[2] = 30;
	lst = make_int_list(vals, 3);

	result = ft_lstmap(lst, f_increment, del_free);

	printf("[2.1] original[0]=10 | got=%d | %s\n",
		vals[0], vals[0] == 10 ? "PASS" : "FAIL");
	printf("[2.2] original[1]=20 | got=%d | %s\n",
		vals[1], vals[1] == 20 ? "PASS" : "FAIL");
	printf("[2.3] original[2]=30 | got=%d | %s\n",
		vals[2], vals[2] == 30 ? "PASS" : "FAIL");
	printf("[2.4] original size=3 | got=%d | %s\n",
		ft_lstsize(lst),
		ft_lstsize(lst) == 3 ? "PASS" : "FAIL");

	ft_lstclear(&result, del_free);
	free_list(lst);
}

/* --- GROUP 3: new list has correct transformed values -------------------- */
static void	test_transformed_values(void)
{
	int		vals[3];
	int		expected[3];
	t_list	*lst;
	t_list	*result;
	t_list	*cur;
	int		i;

	printf("\n=== GROUP 3: transformed values ===\n");

	vals[0] = 1; vals[1] = 2; vals[2] = 3;
	expected[0] = 2; expected[1] = 3; expected[2] = 4;
	lst = make_int_list(vals, 3);

	result = ft_lstmap(lst, f_increment, del_free);

	printf("[3.1] result != NULL | %s\n", result ? "PASS" : "FAIL");
	if (!result)
	{
		free_list(lst);
		return ;
	}

	cur = result;
	i = 0;
	while (cur && i < 3)
	{
		int got = *(int *)cur->content;
		printf("[3.2.%d] expected=%d, got=%d | %s\n",
			i + 1, expected[i], got,
			got == expected[i] ? "PASS" : "FAIL");
		cur = cur->next;
		i++;
	}

	printf("[3.3] result size=3 | got=%d | %s\n",
		ft_lstsize(result),
		ft_lstsize(result) == 3 ? "PASS" : "FAIL");

	ft_lstclear(&result, del_free);
	free_list(lst);
}

/* --- GROUP 4: new list is a deep copy (different pointers) --------------- */
static void	test_new_pointers(void)
{
	int		vals[2];
	t_list	*lst;
	t_list	*result;

	printf("\n=== GROUP 4: new list has different node/content pointers ===\n");

	vals[0] = 5; vals[1] = 6;
	lst = make_int_list(vals, 2);

	result = ft_lstmap(lst, f_increment, del_free);
	if (!result)
	{
		printf("[4.1] result is NULL | FAIL\n");
		free_list(lst);
		return ;
	}

	printf("[4.1] result node != original node | %s\n",
		(void *)result != (void *)lst ? "PASS" : "FAIL");
	printf("[4.2] result->content != original->content | %s\n",
		result->content != lst->content ? "PASS" : "FAIL");

	ft_lstclear(&result, del_free);
	free_list(lst);
}

/* --- GROUP 5: single node ------------------------------------------------- */
static void	test_single_node(void)
{
	int		val;
	t_list	*lst;
	t_list	*result;

	printf("\n=== GROUP 5: single node ===\n");

	val = 99;
	lst = make_node(&val);
	result = ft_lstmap(lst, f_increment, del_free);

	printf("[5.1] result != NULL | %s\n", result ? "PASS" : "FAIL");
	if (result)
	{
		printf("[5.2] value=100 | got=%d | %s\n",
			*(int *)result->content,
			*(int *)result->content == 100 ? "PASS" : "FAIL");
		printf("[5.3] result->next == NULL | %s\n",
			result->next == NULL ? "PASS" : "FAIL");
		ft_lstclear(&result, del_free);
	}
	free(lst);
}

/* --- GROUP 6: string content ---------------------------------------------- */
static void	test_string_content(void)
{
	t_list	*head;
	t_list	*result;
	t_list	*cur;

	printf("\n=== GROUP 6: string content ===\n");

	head = make_node("hello");
	head->next = make_node("world");
	head->next->next = make_node("42");

	result = ft_lstmap(head, f_uppercase, del_free);
	if (!result)
	{
		printf("[6.x] result is NULL | FAIL\n");
		free_list(head);
		return ;
	}

	cur = result;
	printf("[6.1] \"hello\"->\"Hello\" | got=%s | %s\n",
		(char *)cur->content,
		strcmp((char *)cur->content, "Hello") == 0 ? "PASS" : "FAIL");
	cur = cur->next;
	printf("[6.2] \"world\"->\"World\" | got=%s | %s\n",
		(char *)cur->content,
		strcmp((char *)cur->content, "World") == 0 ? "PASS" : "FAIL");
	cur = cur->next;
	printf("[6.3] \"42\"->\"42\"       | got=%s | %s\n",
		(char *)cur->content,
		strcmp((char *)cur->content, "42") == 0 ? "PASS" : "FAIL");

	ft_lstclear(&result, del_free);
	free_list(head);
}

/* --- GROUP 7: f called exactly once per node ----------------------------- */
static void	test_f_call_count(void)
{
	int		vals[5];
	t_list	*lst;
	t_list	*result;
	int		i;

	printf("\n=== GROUP 7: f called once per node ===\n");

	i = 0;
	while (i < 5)
	{
		vals[i] = i;
		i++;
	}
	lst = make_int_list(vals, 5);

	g_f_count = 0;
	result = ft_lstmap(lst, f_increment, del_free);
	printf("[7.1] f called 5 times | got=%d | %s\n",
		g_f_count, g_f_count == 5 ? "PASS" : "FAIL");

	ft_lstclear(&result, del_free);
	free_list(lst);
}

/* --- GROUP 8: f returns NULL -> cleanup and return NULL ------------------ */
/*                                                                            */
/*   ft_lstnew(NULL) creates a node with NULL content.                        */
/*   If your ft_lstmap treats that as a failure: result must be NULL          */
/*   and del must have been called on already-built nodes.                    */
/*   Either way: no crash, no leak.                                           */
/* -------------------------------------------------------------------------- */
static void	test_f_returns_null(void)
{
	int		vals[3];
	t_list	*lst;
	t_list	*result;

	printf("\n=== GROUP 8: f returns NULL ===\n");

	vals[0] = 1; vals[1] = 2; vals[2] = 3;
	lst = make_int_list(vals, 3);

	g_del_count = 0;
	result = ft_lstmap(lst, f_returns_null, del_count);

	printf("[8.1] no crash | PASS\n");
	printf("[8.2] result=%s (NULL if NULL treated as alloc failure)\n",
		result ? "non-NULL" : "NULL");
	if (result)
		ft_lstclear(&result, del_noop);

	free_list(lst);
}

/* --- GROUP 9: identity f, content values match original ------------------ */
static void	test_identity(void)
{
	int		vals[3];
	t_list	*lst;
	t_list	*result;
	t_list	*rcur;
	t_list	*ocur;
	int		idx;

	printf("\n=== GROUP 9: identity f ===\n");

	vals[0] = 7; vals[1] = 8; vals[2] = 9;
	lst = make_int_list(vals, 3);

	result = ft_lstmap(lst, f_identity, del_noop);
	if (!result)
	{
		printf("[9.x] result is NULL | FAIL\n");
		free_list(lst);
		return ;
	}

	rcur = result;
	ocur = lst;
	idx = 1;
	while (rcur && ocur)
	{
		int rv = *(int *)rcur->content;
		int ov = *(int *)ocur->content;
		printf("[9.%d] expected=%d, got=%d | %s\n",
			idx, ov, rv, rv == ov ? "PASS" : "FAIL");
		rcur = rcur->next;
		ocur = ocur->next;
		idx++;
	}

	printf("[9.4] result node != original node | %s\n",
		(void *)result != (void *)lst ? "PASS" : "FAIL");

	ft_lstclear(&result, del_noop);
	free_list(lst);
}

/* --- GROUP 10: last node->next is NULL ------------------------------------ */
static void	test_last_next_null(void)
{
	int		vals[4];
	t_list	*lst;
	t_list	*result;
	t_list	*cur;

	printf("\n=== GROUP 10: last node->next == NULL ===\n");

	vals[0] = 1; vals[1] = 2; vals[2] = 3; vals[3] = 4;
	lst = make_int_list(vals, 4);

	result = ft_lstmap(lst, f_increment, del_free);
	if (!result)
	{
		printf("[10.1] result is NULL | FAIL\n");
		free_list(lst);
		return ;
	}

	cur = result;
	while (cur->next)
		cur = cur->next;
	printf("[10.1] last->next == NULL | %s\n",
		cur->next == NULL ? "PASS" : "FAIL");

	ft_lstclear(&result, del_free);
	free_list(lst);
}

/* ========================================================================= */
int	main(void)
{
	test_null_guards();
	test_original_untouched();
	test_transformed_values();
	test_new_pointers();
	test_single_node();
	test_string_content();
	test_f_call_count();
	test_f_returns_null();
	test_identity();
	test_last_next_null();
	printf("\nDone.\n");
	return (0);
}