/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:54:43 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 14:28:08 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Prints error msg specific to threads based on an ID into stderr
 */
void thread_errors(int error_id, int index)
{
	fprintf(stderr, "\033[0;31m");

	if (error_id == 1)
		fprintf(stderr, "Failed to create thread number: {%d}\n", index);
	else if (error_id == 2)
		fprintf(stderr, "Failed to join thread number: {%d}\n", index);

	fprintf(stderr, "\033[0m");
}

/**
 * @brief Prints error msg specific to mutexes based on an ID into stderr
 */
void mutex_errors(int error_id, int index)
{
	fprintf(stderr, "\033[0;31m");

	if (error_id == 1)
		fprintf(stderr, "Failed to initialize mutex with id: {%d}\n", index);
	else if (error_id == 2)
		fprintf(stderr, "Failed to destroy mutex with id: {%d}\n", index);

	fprintf(stderr, "\033[0m");
}