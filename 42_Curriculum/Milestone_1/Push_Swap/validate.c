#include "push_swap.h"

static void	check_flag(int argc, char *argv[], t_flags *flags);
static void	check_int(int argc, char *argv[]);
static int	check_duplicate_argv(int argc, char *argv[], int **unsorted_lst);
static int	int_array(int argc, char *argv[], int **unsorted_lst);

/**
 * @brief  Validates all command-line arguments and populates the unsorted list.
 *
 * @param  argc          int      — Argument count from main. 
 * @param  argv          char*[]  — Argument vector from main.
 * @param  unsorted_lst  int**    — Output parameter with a heap-allocated
 * 									array of parsed integers.
 * @param  flags         t_flags* — Pointer to a flags struct populated by 
 * 									`check_flag`.
 *
 * @return int — The number of valid integer elements parsed from argv, 
 * 				 as returned by `check_duplicate_argv`.
 *
 * @note  More than 2 flags is always rejected; exactly 2 flags
 *        is only accepted if `flags->bench` is set.
 */
int	check_argv(int argc, char *argv[], int **unsorted_lst, t_flags *flags)
{
	int	n_elements;

	check_flag(argc, argv, flags);
	if (flags->n_flags > 2 || (flags->n_flags == 2 && flags->bench == false))
		print_error();
	check_int(argc, argv);
	n_elements = check_duplicate_argv(argc, argv, unsorted_lst);
	return (n_elements);
}

/**
 * @brief  Parses and records recognized double-dash flags 
 * 		   from argv into a flags struct.
 *
 * @details  Iterates over argv starting at index 1, processing only 
 * 			 entries that begin with "--".
 */
static void	check_flag(int argc, char *argv[], t_flags *flags)
{
	size_t	i;

	i = 1;
	while (i < (unsigned long)argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			if (!ft_strcmp(argv[i], "--simple"))
				flags->sys_strat = SIMPLE;
			else if (!ft_strcmp(argv[i], "--medium"))
				flags->sys_strat = MEDIUM;
			else if (!ft_strcmp(argv[i], "--complex"))
				flags->sys_strat = COMPLEX;
			else if (!ft_strcmp(argv[i], "--adaptive"))
				flags->sys_strat = ADAPTIVE;
			else if (flags->bench == false && !ft_strcmp(argv[i], "--bench"))
				flags->bench = true;
			else
				print_error();
			flags->n_flags += 1;
		}
		i++;
	}
}

/**
 * @brief  Validates that all non-flag argv entries are integers 
 * 		   within int range.
 *
 * @details  Iterates over argv starting at index 1, skipping entries that begin
 *           with "--" (double-dash flags). For all other entries, parses the 
 * 			 value via `ft_atoi_strict` and calls `print_error` if the result
 * 			 falls outside the range [INT_MIN, INT_MAX].
 *
 * @return void — no return value.
 */
static void	check_int(int argc, char *argv[])
{
	size_t	i;
	long	n;

	i = 1;
	while (i < (unsigned long)argc)
	{
		if (argv[i][0] != '-' || argv[i][1] != '-')
		{
			n = ft_atoi_strict(argv[i]);
			if (n < INT_MIN || n > INT_MAX)
				print_error();
		}
		i++;
	}
}

/**
 * @brief  Builds the integer array from argv and rejects any duplicate values.
 *
 * @return int — The number of unique integer elements parsed from argv.
 */
static int	check_duplicate_argv(int argc, char *argv[], int **unsorted_lst)
{
	size_t	i;
	size_t	j;
	int		n_elements;

	n_elements = int_array(argc, argv, unsorted_lst);
	i = 0;
	while (i < (size_t)n_elements)
	{
		j = i + 1;
		while (j < (size_t)n_elements)
		{
			if ((*unsorted_lst)[i] == (*unsorted_lst)[j])
			{
				free (*unsorted_lst);
				print_error();
			}
			j++;
		}
		i++;
	}
	return (n_elements);
}

/**
 * @brief  Counts non-flag argv entries, allocates an integer array,
 * 		   and fills it.
 *
 * @details  Makes two passes over argv. The first pass counts all entries 
 * 		     that do not begin with "--", which determines the allocation 
 * 			 size. After allocating via `ft_calloc`, the second pass fills
 * 			 the array by parsing each non-flag entry with `ft_atoi`. 
 * 			 Returns the number of elements stored in `*unsorted_lst`.
 *
 * @return int — The number of integer elements parsed and stored in 
 * 				 `*unsorted_lst`. Never returns on allocation 
 * 				 failure — `print_error` is called instead. [INFERRED]
 */
static int	int_array(int argc, char *argv[], int **unsorted_lst)
{
	int		n_elements;
	size_t	i;
	size_t	j;

	i = 1;
	n_elements = 0;
	while (i < (unsigned long)argc)
	{
		if (argv[i][0] != '-' || argv[i][1] != '-')
			n_elements++;
		i++;
	}
	*unsorted_lst = ft_calloc(n_elements, sizeof(int));
	if (!*unsorted_lst)
		print_error();
	i = 1;
	j = 0;
	while (i < (unsigned long)argc)
	{
		if (argv[i][0] != '-' || argv[i][1] != '-')
			(*unsorted_lst)[j++] = ft_atoi(argv[i]);
		i++;
	}
	return (n_elements);
}
