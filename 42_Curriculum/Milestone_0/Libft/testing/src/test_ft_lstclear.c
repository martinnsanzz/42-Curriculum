#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

/* ─── t_list struct ─────────────────────────────────────────── */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/* ─── Prototypes (your functions) ──────────────────────────── */
t_list	*ft_lstnew(void *content);
void	ft_lstclear(t_list **lst, void (*del)(void *));

/* ─── Helpers ───────────────────────────────────────────────── */
static int	g_del_count = 0;

static void	del_free(void *content)
{
	free(content);
	g_del_count++;
}

static void	del_noop(void *content)
{
	(void)content;
	g_del_count++;
}

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

static t_list	*build_list(char **strings, int count)
{
	t_list	*head;
	t_list	*cur;
	int		i;

	head = NULL;
	i = 0;
	while (i < count)
	{
		t_list *node = make_node(strdup(strings[i]));
		if (!head)
		{
			head = node;
			cur = head;
		}
		else
		{
			cur->next = node;
			cur = cur->next;
		}
		i++;
	}
	return (head);
}

static int	list_len(t_list *lst)
{
	int	n;

	n = 0;
	while (lst)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

/* ─── Test macros ───────────────────────────────────────────── */
#define PASS "\033[32m[PASS]\033[0m"
#define FAIL "\033[31m[FAIL]\033[0m"

/* ══════════════════════════════════════════════════════════════
** TEST 1 – NULL list pointer (must not crash)
** ══════════════════════════════════════════════════════════════ */
static void	test_null_list(void)
{
	printf("\n=== TEST 1: ft_lstclear(NULL, del) ===\n");
	ft_lstclear(NULL, del_noop);
	printf("  No crash: %s\n", PASS);
}

/* ══════════════════════════════════════════════════════════════
** TEST 2 – Pointer-to-NULL list (*lst == NULL)
** ══════════════════════════════════════════════════════════════ */
static void	test_empty_list(void)
{
	t_list	*lst;

	printf("\n=== TEST 2: ft_lstclear(&lst, del) where lst == NULL ===\n");
	lst = NULL;
	g_del_count = 0;
	ft_lstclear(&lst, del_noop);
	printf("  lst == NULL after: %s\n", lst == NULL ? PASS : FAIL);
	printf("  del called 0 times (got %d): %s\n",
		g_del_count, g_del_count == 0 ? PASS : FAIL);
}

/* ══════════════════════════════════════════════════════════════
** TEST 3 – Single node, heap content
** ══════════════════════════════════════════════════════════════ */
static void	test_single_node(void)
{
	t_list	*lst;

	printf("\n=== TEST 3: Single node with heap content ===\n");
	lst = make_node(strdup("hello"));
	g_del_count = 0;
	ft_lstclear(&lst, del_free);
	printf("  lst == NULL after: %s\n", lst == NULL ? PASS : FAIL);
	printf("  del called 1 time (got %d): %s\n",
		g_del_count, g_del_count == 1 ? PASS : FAIL);
}

/* ══════════════════════════════════════════════════════════════
** TEST 4 – Multi-node list (5 nodes)
** ══════════════════════════════════════════════════════════════ */
static void	test_multi_node(void)
{
	t_list	*lst;
	char	*words[] = {"one", "two", "three", "four", "five"};

	printf("\n=== TEST 4: 5-node list ===\n");
	lst = build_list(words, 5);
	printf("  Nodes before clear: %d\n", list_len(lst));
	g_del_count = 0;
	ft_lstclear(&lst, del_free);
	printf("  lst == NULL after: %s\n", lst == NULL ? PASS : FAIL);
	printf("  del called 5 times (got %d): %s\n",
		g_del_count, g_del_count == 5 ? PASS : FAIL);
}

/* ══════════════════════════════════════════════════════════════
** TEST 5 – Node with NULL content (del must still be called)
** ══════════════════════════════════════════════════════════════ */
static void	test_null_content(void)
{
	t_list	*lst;

	printf("\n=== TEST 5: Node with NULL content ===\n");
	lst = make_node(NULL);
	g_del_count = 0;
	ft_lstclear(&lst, del_noop);
	printf("  lst == NULL after: %s\n", lst == NULL ? PASS : FAIL);
	printf("  del called 1 time (got %d): %s\n",
		g_del_count, g_del_count == 1 ? PASS : FAIL);
}

/* ══════════════════════════════════════════════════════════════
** TEST 6 – Clear, then clear again (idempotent)
** ══════════════════════════════════════════════════════════════ */
static void	test_double_clear(void)
{
	t_list	*lst;
	char	*words[] = {"a", "b", "c"};

	printf("\n=== TEST 6: Clear twice (idempotent) ===\n");
	lst = build_list(words, 3);
	ft_lstclear(&lst, del_free);
	g_del_count = 0;
	ft_lstclear(&lst, del_noop);
	printf("  lst still NULL: %s\n", lst == NULL ? PASS : FAIL);
	printf("  del not called again (got %d): %s\n",
		g_del_count, g_del_count == 0 ? PASS : FAIL);
}

/* ══════════════════════════════════════════════════════════════
** TEST 7 – Large list (1000 nodes)
** ══════════════════════════════════════════════════════════════ */
static void	test_large_list(void)
{
	t_list	*lst;
	t_list	*cur;
	int		i;

	printf("\n=== TEST 7: 1000-node list ===\n");
	lst = NULL;
	cur = NULL;
	i = 0;
	while (i < 1000)
	{
		t_list *node = make_node(strdup("x"));
		if (!lst)
		{
			lst = node;
			cur = lst;
		}
		else
		{
			cur->next = node;
			cur = cur->next;
		}
		i++;
	}
	g_del_count = 0;
	ft_lstclear(&lst, del_free);
	printf("  lst == NULL after: %s\n", lst == NULL ? PASS : FAIL);
	printf("  del called 1000 times (got %d): %s\n",
		g_del_count, g_del_count == 1000 ? PASS : FAIL);
}

/* ══════════════════════════════════════════════════════════════
** TEST 8 – Verify del is called BEFORE free (order check)
**          Uses a flag inside content to detect use-after-free
** ══════════════════════════════════════════════════════════════ */
static int	g_del_saw_value = 0;

static void	del_check_value(void *content)
{
	if (content && *(int *)content == 42)
		g_del_saw_value = 1;
	free(content);
}

static void	test_del_before_free(void)
{
	t_list	*lst;
	int		*val;

	printf("\n=== TEST 8: del called before node is freed ===\n");
	val = malloc(sizeof(int));
	*val = 42;
	lst = make_node(val);
	g_del_saw_value = 0;
	ft_lstclear(&lst, del_check_value);
	printf("  del received valid content (saw 42): %s\n",
		g_del_saw_value ? PASS : FAIL);
	printf("  lst == NULL: %s\n", lst == NULL ? PASS : FAIL);
}

/* ─── main ──────────────────────────────────────────────────── */
int	main(void)
{
	test_null_list();
	test_empty_list();
	test_single_node();
	test_multi_node();
	test_null_content();
	test_double_clear();
	test_large_list();
	test_del_before_free();
	printf("\n");
	return (0);
}