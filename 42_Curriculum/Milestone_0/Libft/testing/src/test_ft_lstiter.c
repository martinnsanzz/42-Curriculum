#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

/* ---- Minimal t_list definition (skip if using your libft.h) ------------- */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/* ---- Function under test ------------------------------------------------- */
void	ft_lstiter(t_list *lst, void (*f)(void *));

/* ---- Helpers ------------------------------------------------------------- */

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

/* ---- Counter: tracks how many times f was called ------------------------ */
static int	g_call_count = 0;

static void	count_calls(void *content)
{
	(void)content;
	g_call_count++;
}

/* ---- Modifier: uppercases first char of a char* content ----------------- */
static void	uppercase_first(void *content)
{
	char	*s;

	s = (char *)content;
	if (s && s[0] >= 'a' && s[0] <= 'z')
		s[0] -= 32;
}

/* ---- Modifier: increments an int* content -------------------------------- */
static void	increment_int(void *content)
{
	int	*n;

	n = (int *)content;
	(*n)++;
}

/* ---- Modifier: sets content to NULL (stress test) ----------------------- */
static void	set_null_content(void *content)
{
	(void)content;
	/* f receives the content value, not the node pointer.
	   ft_lstiter must NOT crash when f does nothing dangerous. */
}

/* ---- NULL-content node: f must receive NULL without crashing ------------ */
static void	handle_null_content(void *content)
{
	if (content == NULL)
		g_call_count++;
}

/* ========================================================================= */
/*   TESTS                                                                    */
/* ========================================================================= */

/* --- GROUP 1: NULL guards ------------------------------------------------- */
static void	test_null_list(void)
{
	printf("=== GROUP 1: NULL guards ===\n");

	/* 1.1 NULL list, valid f */
	g_call_count = 0;
	ft_lstiter(NULL, count_calls);
	printf("[1.1] lst=NULL, f=count_calls | calls expected=0, got=%d | %s\n",
		g_call_count, g_call_count == 0 ? "PASS" : "FAIL");

	/* 1.2 valid list, f=NULL */
	{
		t_list	*lst;
		int		val;

		val = 42;
		lst = make_node(&val);
		g_call_count = 0;
		ft_lstiter(lst, NULL);
		printf("[1.2] f=NULL, valid list   | calls expected=0, got=%d | %s\n",
			g_call_count, g_call_count == 0 ? "PASS" : "FAIL");
		free_list(lst);
	}

	/* 1.3 NULL list, NULL f */
	g_call_count = 0;
	ft_lstiter(NULL, NULL);
	printf("[1.3] lst=NULL, f=NULL      | calls expected=0, got=%d | %s\n",
		g_call_count, g_call_count == 0 ? "PASS" : "FAIL");
}

/* --- GROUP 2: single node ------------------------------------------------- */
static void	test_single_node(void)
{
	t_list	*lst;
	int		val;

	printf("\n=== GROUP 2: single node ===\n");

	val = 10;
	lst = make_node(&val);
	ft_lstiter(lst, increment_int);
	printf("[2.1] single int node, increment | expected=11, got=%d | %s\n",
		val, val == 11 ? "PASS" : "FAIL");
	free_list(lst);
}

/* --- GROUP 3: multiple nodes ---------------------------------------------- */
static void	test_multiple_nodes(void)
{
	t_list	*head;
	int		a;
	int		b;
	int		c;

	printf("\n=== GROUP 3: multiple nodes ===\n");

	a = 1; b = 2; c = 3;
	head = make_node(&a);
	head->next = make_node(&b);
	head->next->next = make_node(&c);

	/* 3.1 call count equals list length */
	g_call_count = 0;
	ft_lstiter(head, count_calls);
	printf("[3.1] 3-node list, call count | expected=3, got=%d | %s\n",
		g_call_count, g_call_count == 3 ? "PASS" : "FAIL");

	/* 3.2 every node is modified */
	ft_lstiter(head, increment_int);
	printf("[3.2] every node incremented  | expected=2,3,4 | got=%d,%d,%d | %s\n",
		a, b, c,
		(a == 2 && b == 3 && c == 4) ? "PASS" : "FAIL");

	free_list(head);
}

/* --- GROUP 4: string content ---------------------------------------------- */
static void	test_string_content(void)
{
	char	s1[] = "hello";
	char	s2[] = "world";
	char	s3[] = "42";
	t_list	*head;

	printf("\n=== GROUP 4: string content ===\n");

	head = make_node(s1);
	head->next = make_node(s2);
	head->next->next = make_node(s3);

	ft_lstiter(head, uppercase_first);
	printf("[4.1] uppercase_first on strings | expected=Hello,World,42 | got=%s,%s,%s | %s\n",
		s1, s2, s3,
		(strcmp(s1, "Hello") == 0 && strcmp(s2, "World") == 0
			&& strcmp(s3, "42") == 0) ? "PASS" : "FAIL");

	free_list(head);
}

/* --- GROUP 5: NULL content inside node ------------------------------------ */
static void	test_null_content(void)
{
	t_list	*lst;

	printf("\n=== GROUP 5: NULL content in node ===\n");

	lst = make_node(NULL);
	g_call_count = 0;
	/* f must be called once even if content is NULL */
	ft_lstiter(lst, handle_null_content);
	printf("[5.1] node with NULL content  | f called with NULL expected=1, got=%d | %s\n",
		g_call_count, g_call_count == 1 ? "PASS" : "FAIL");

	free_list(lst);
}

/* --- GROUP 6: f does nothing (no-op) -------------------------------------- */
static void	test_noop_f(void)
{
	t_list	*head;
	int		a;
	int		b;

	printf("\n=== GROUP 6: no-op function, values unchanged ===\n");

	a = 7; b = 8;
	head = make_node(&a);
	head->next = make_node(&b);

	ft_lstiter(head, set_null_content);
	printf("[6.1] no-op f, a unchanged | expected=7, got=%d | %s\n",
		a, a == 7 ? "PASS" : "FAIL");
	printf("[6.2] no-op f, b unchanged | expected=8, got=%d | %s\n",
		b, b == 8 ? "PASS" : "FAIL");

	free_list(head);
}

/* --- GROUP 7: large list -------------------------------------------------- */
static void	test_large_list(void)
{
	t_list	*head;
	t_list	*cur;
	t_list	*prev;
	int		vals[1000];
	int		i;
	int		pass;

	printf("\n=== GROUP 7: large list (1000 nodes) ===\n");

	i = 0;
	head = NULL;
	prev = NULL;
	while (i < 1000)
	{
		vals[i] = i;
		cur = make_node(&vals[i]);
		if (!head)
			head = cur;
		else
			prev->next = cur;
		prev = cur;
		i++;
	}

	ft_lstiter(head, increment_int);

	pass = 1;
	i = 0;
	while (i < 1000)
	{
		if (vals[i] != i + 1)
		{
			pass = 0;
			break ;
		}
		i++;
	}
	printf("[7.1] 1000 nodes all incremented | %s\n", pass ? "PASS" : "FAIL");

	g_call_count = 0;
	ft_lstiter(head, count_calls);
	printf("[7.2] call count = 1000 | got=%d | %s\n",
		g_call_count, g_call_count == 1000 ? "PASS" : "FAIL");

	free_list(head);
}

/* ========================================================================= */
int	main(void)
{
	test_null_list();
	test_single_node();
	test_multiple_nodes();
	test_string_content();
	test_null_content();
	test_noop_f();
	test_large_list();
	printf("\nDone.\n");
	return (0);
}