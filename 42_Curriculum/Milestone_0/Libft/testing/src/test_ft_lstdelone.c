#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libft.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/* ---- your function prototype --------------------------------------------- */

void	ft_lstdelone(t_list *lst, void (*del)(void *));

/* ---- helpers ------------------------------------------------------------- */

static int	g_del_called = 0;
static void	*g_del_ptr = NULL;

static void	del_track(void *content)
{
	g_del_called++;
	g_del_ptr = content;
	free(content);
}

static void	del_noop(void *content)
{
	g_del_called++;
	(void)content;
}

static t_list	*make_node(void *content, t_list *next)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = next;
	return (node);
}

static void	print_result(const char *test, int pass)
{
	if (pass)
		printf("  [PASS] %s\n", test);
	else
		printf("  [FAIL] %s\n", test);
}

/* ---- test sections ------------------------------------------------------- */

/*
** 1. del() is called exactly once with the correct content pointer.
*/
static void	test_del_called(void)
{
	char	*data;
	t_list	*node;

	printf("\n=== del() invocation ===\n");

	data = strdup("hello");
	node = make_node(data, NULL);

	g_del_called = 0;
	g_del_ptr = NULL;
	ft_lstdelone(node, del_track);

	print_result("del called exactly once", g_del_called == 1);
	print_result("del received correct pointer", g_del_ptr == data);
	/* node itself is freed by ft_lstdelone — do NOT access it here */
}

/*
** 2. The node struct itself is freed.
**    We use valgrind / leak sanitizer to verify — no direct check possible
**    without UB. We just confirm the call doesn't crash.
*/
static void	test_node_freed(void)
{
	char	*data;
	t_list	*node;

	printf("\n=== node struct is freed (no crash / no leak) ===\n");

	data = strdup("world");
	node = make_node(data, NULL);

	g_del_called = 0;
	ft_lstdelone(node, del_track);

	print_result("no crash on free", 1);
	print_result("del was called (content freed)", g_del_called == 1);
}

/*
** 3. next node is NOT freed or touched.
**    We keep a pointer to the next node and verify it is still accessible.
*/
static void	test_next_untouched(void)
{
	char	*d1;
	char	*d2;
	t_list	*next;
	t_list	*node;

	printf("\n=== next node NOT freed ===\n");

	d1 = strdup("first");
	d2 = strdup("second");
	next = make_node(d2, NULL);
	node = make_node(d1, next);

	g_del_called = 0;
	ft_lstdelone(node, del_track);

	/* next must still be accessible and intact */
	print_result("next node still accessible", next != NULL);
	print_result("next->content intact", strcmp((char *)next->content, "second") == 0);
	print_result("del called only once (next untouched)", g_del_called == 1);

	/* clean up next manually */
	free(next->content);
	free(next);
}

/*
** 4. del receives NULL content (content field is NULL).
**    del must be called — it is del's responsibility to handle NULL.
**    ft_lstdelone must not skip the call.
*/
static void	test_null_content(void)
{
	t_list	*node;

	printf("\n=== NULL content ===\n");

	node = make_node(NULL, NULL);

	g_del_called = 0;
	ft_lstdelone(node, del_noop);

	print_result("del called even with NULL content", g_del_called == 1);
}

/*
** 5. NULL lst: must not crash (undefined behavior risk — test documents it).
**    42 subject does not specify this, but evaluators may test it.
**    Uncomment only if your implementation handles NULL lst gracefully.
*/
static void	test_null_lst(void)
{
	printf("\n=== NULL lst (guard check) ===\n");
	printf("  [INFO] Calling ft_lstdelone(NULL, del_noop) — expect no crash\n");

	g_del_called = 0;
	ft_lstdelone(NULL, del_noop);

	print_result("no crash on NULL lst", 1);
	print_result("del NOT called when lst is NULL", g_del_called == 0);
}

/*
** 6. Chain of nodes — delete only the middle one.
**    Verifies isolation: head and tail remain valid.
*/
static void	test_middle_node(void)
{
	char	*d1;
	char	*d2;
	char	*d3;
	t_list	*head;
	t_list	*mid;
	t_list	*tail;

	printf("\n=== delete middle node in a chain ===\n");

	d1 = strdup("A");
	d2 = strdup("B");
	d3 = strdup("C");
	tail = make_node(d3, NULL);
	mid  = make_node(d2, tail);
	head = make_node(d1, mid);

	g_del_called = 0;
	ft_lstdelone(mid, del_track);

	print_result("del called once", g_del_called == 1);
	print_result("head->content intact", strcmp((char *)head->content, "A") == 0);
	print_result("tail->content intact", strcmp((char *)tail->content, "C") == 0);

	/* link head to tail manually (lstdelone doesn't patch the list) */
	head->next = tail;
	print_result("head->next can be relinked to tail", head->next == tail);

	free(head->content);
	free(head);
	free(tail->content);
	free(tail);
}

/* ---- main ---------------------------------------------------------------- */

int	main(void)
{
	printf("========================================\n");
	printf("        ft_lstdelone tester\n");
	printf("========================================\n");

	test_del_called();
	test_node_freed();
	test_next_untouched();
	test_null_content();
	test_null_lst();       /* comment out if no NULL guard in your impl */
	test_middle_node();

	printf("\n========================================\n");
	printf("  Run with valgrind to check for leaks\n");
	printf("========================================\n");
	return (0);
}