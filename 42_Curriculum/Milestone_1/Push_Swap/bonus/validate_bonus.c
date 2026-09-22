#include "push_swap_bonus.h"

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
 */
int	check_argv(int argc, char *argv[], int **unsorted_lst)
{
	int	n_elements;

	check_int(argc, argv);
	n_elements = check_duplicate_argv(argc, argv, unsorted_lst);
	return (n_elements);
}

/**
 * @brief  Validates that all non-flag argv entries are integers 
 * 		   within int range.
 *
 * @details  Iterates over argv starting at index 1. Parses the 
 * 			 value via `ft_atoi_strict` and calls `print_error` if the result
 * 			 falls outside the range [INT_MIN, INT_MAX].
 */
static void	check_int(int argc, char *argv[])
{
	size_t	i;
	long	n;

	i = 1;
	while (i < (unsigned long)argc)
	{
		n = ft_atoi_strict(argv[i]);
		if (n < INT_MIN || n > INT_MAX)
			print_error();
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
 * @brief  Counts argv entries, allocates an integer array,
 * 		   and fills it.
 *
 * @details  Makes two passes over argv. The first pass counts all entries, 
 * 			 which determines the allocation size. After allocating 
 * 			 via `ft_calloc`, the second pass fills the array by parsing each 
 * 			 non-flag entry with `ft_atoi`. 
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
		(*unsorted_lst)[j++] = ft_atoi(argv[i]);
		i++;
	}
	return (n_elements);
}
