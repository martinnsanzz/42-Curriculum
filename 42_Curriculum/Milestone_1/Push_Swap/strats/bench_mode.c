#include "../push_swap.h"

static void	print_disorder(float disorder);
static void	print_strat(char *strat, char *big_o);
static void	print_operations(char *text, int num, int new_line);

/**
 * @brief Prints the whole alghoritm stats to stdeer as per the pdf
 */
void	bench(t_flags flags, t_operations op, float disorder)
{
	char	*strats[4];
	char	*big_o[3];

	strats[0] = "Adaptive";
	strats[1] = "Simple";
	strats[2] = "Medium";
	strats[3] = "Complex";
	big_o[0] = "O(n^2)";
	big_o[1] = "O(n√n)";
	big_o[2] = "O(nlogn)";
	print_disorder(disorder);
	print_strat(strats[flags.sys_strat], big_o[flags.user_strat - 1]);
	print_operations("[bench] total_ops: ", sum_operations(op), 1);
	print_operations("[bench] sa:  ", op.sa, 0);
	print_operations(" sb: ", op.sb, 0);
	print_operations(" ss: ", op.ss, 0);
	print_operations(" pa: ", op.pa, 0);
	print_operations(" pb: ", op.pb, 1);
	print_operations("[bench] ra: ", op.ra, 0);
	print_operations(" rb: ", op.rb, 0);
	print_operations(" rr: ", op.rr, 0);
	print_operations(" rra: ", op.rra, 0);
	print_operations(" rrb: ", op.rrb, 0);
	print_operations(" rrr: ", op.rrr, 1);
}

/**
 * @brief Converts the disorder (float) into a percentage from
 * 		  0% to 100% and prints it
 */
static void	print_disorder(float disorder)
{
	int		percentage;
	int		fractional_digits;

	ft_putstr_fd("[bench] disorder: ", 2);
	if (disorder == 1)
	{
		ft_putendl_fd("100.00%", 2);
		return ;
	}
	else if (disorder == 0)
	{
		ft_putendl_fd("00.00%", 2);
		return ;
	}
	percentage = (int)(disorder * 100);
	fractional_digits = ((disorder * 100) - percentage) * 100;
	ft_putnbr_fd(percentage, 2);
	ft_putchar_fd('.', 2);
	if (fractional_digits < 10)
		ft_putnbr_fd(0, 2);
	ft_putnbr_fd(fractional_digits, 2);
	ft_putstr_fd("%\n", 2);
}

static void	print_strat(char *strat, char *big_o)
{
	ft_putstr_fd("[bench] strategy: ", 2);
	ft_putstr_fd(strat, 2);
	ft_putstr_fd(" / ", 2);
	ft_putendl_fd(big_o, 2);
}

static void	print_operations(char *text, int num, int new_line)
{
	ft_putstr_fd(text, 2);
	ft_putnbr_fd(num, 2);
	if (new_line == 1)
		ft_putendl_fd("", 2);
}
