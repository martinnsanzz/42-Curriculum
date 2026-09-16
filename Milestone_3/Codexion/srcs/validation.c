/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 12:16:06 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 15:16:38 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_isnumber(char *s);

/**
 * @brief Checks that all CLI follow the expected values.
 *
 * Expected values are:
 * 	- Program must take 8 in total.
 * 	- First 7 arguments must be numbers (valid number explained
 * 										 in future func).
 * 	- Argument 8 must be the correct scheduler (fifo or edf).
 *
 * @param argc Argument count.
 * @param argv Arguments in an array of strings.
 *
 * @return 0 on parsing success.
 * @return 1 on error found in any argument.
 */
int     check_argv(int argc, char *argv[])
{
	if (argc != 9)
		return (invalid_num_of_args(), 1);
	else if ((ft_strcmp(argv[8], "fifo")) && (ft_strcmp(argv[8], "edf")))
		return (wrong_scheduler(argv[8]), 1);

	if (check_valid_num(argv) == 1)
		return (1);
	return (0);
}

/**
 * @brief Checks if argumen is a valid number.
 *
 * If not a valid number it checks and redirects to the correct
 * error msg.
 *
 * @param argv CLI arguments to check.
 *
 * @return 0 on success.
 * @return 1 if number is not valid.
 */
int check_valid_num(char *argv[])
{
	int i;

    i = 0;
	while (i++ < 7)
	{
		if (ft_isnumber(argv[i]) == 1)
			return (invalid_num(1, i, argv[i]), 1);
		else if (ft_atoi(argv[i]) == 0 && ft_strlen(argv[i]) > 1)
			return (invalid_num(2, i, argv[i]), 1);
		else if (ft_atoi(argv[i]) == 0 && ft_strlen(argv[i]) == 1)
			return (invalid_num(3, i, argv[i]), 1);
		else if (ft_atoi(argv[i]) < 0)
			return (invalid_num(4, i, argv[i]), 1);
	}
	return (0);
}

/**
 * @brief Extracts the CLI and packs them in @p prog.
 *
 * It takes all int values from 1 - 7 into an int array
 * and the scheduler which is value 8.
 *
 * @param argv CLI arguments.
 * @param prog Pointer to the program struct holding the
 * 			   program data.
 */
void    get_rules(char *argv[], t_program *prog)
{
    int i;

    i = 0;
    while(i++ < 7)
		(*prog).rules[i - 1] = (int)ft_atoi(argv[i]);
	(*prog).scheduler = argv[8];
}

/**
 * @brief Checks if a string would be consider a valid number.
 *
 * Is consider a valid number if:
 * 	- All characters of the string are digits (0-9).
 * 	- If char '-' is in the first position (Negative value).
 *
 * @param s String to check.
 *
 * @return 0 if @p s is a valid number.
 * @return 1 if @p s is an invalid number.
 */
static int	ft_isnumber(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		if (!(s[i] >= '0' && s[i] <= '9') && (s[i] == '-' && i != 0))
				return (1);
		i++;
	}
	return (0);
}
