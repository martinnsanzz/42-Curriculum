/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_errors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 12:02:04 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 13:17:29 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Prints error msg if number of args is not 8
 */
void invalid_num_of_args()
{
	fprintf(stderr, "\033[0;31m");
	fprintf(stderr, "Program must have 8 arguments !!\n "
					"Run 'make help-run'.\n");
	fprintf(stderr, "\033[0m");
}

/**
 * @brief Prints error msg if number is not valid
 *
 * Is considered an invalid number if:
 * 	- Is not a valid int (No float)
 * 	- Is negative
 * 	- Is larger than INT_MAX
 * 	- Is number of coders is == 0
 */
void invalid_num(int error_id, int index, char *value)
{
	fprintf(stderr, "\033[0;31m");
	if (error_id == 1)
	{
		fprintf(stderr, "Argument %d is an invalid "
						"int: {%s}\n", index, value);
	}
	else if (error_id == 2)
	{
		fprintf(stderr, "Value '%d' can't be a greater "
						"than INT_MAX: {%s}\n", index, value);
	}
	else if (error_id == 3)
	{
		fprintf(stderr, "Value '%d' can't be a negative "
			"value: {%s}\n", index, value);
	}
	(void)index;
	(void)value;
	if (error_id == 4)
		fprintf(stderr, "Number of coders must be greater than 0\n");
	fprintf(stderr, "\033[0m");
}

/**
 * @brief Prints error msg if the scheduler is one of the expected.
 */
void wrong_scheduler(char *value)
{
	fprintf(stderr, "\033[0;31m");
	fprintf(stderr, "Wrong scheduler {%s}: Allowed ['fifo', 'edf']\n", value);
	fprintf(stderr, "\033[0m");
}