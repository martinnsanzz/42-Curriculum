/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:54:43 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/24 15:17:18 by masanz-s         ###   ########.fr       */
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
	(void)index;
	if (error_id == 3)
		fprintf(stderr, "Failed to create monitor thread\n");
	else if (error_id == 4)
		fprintf(stderr, "Failed to join monitor thread\n");
	fprintf(stderr, "\033[0m");
}

/**
 * @brief Prints error msg specific to mutexes based on an ID into stderr
 */
void mutex_init_errors(int error_id, int index)
{
	fprintf(stderr, "\033[0;31m");
	if (error_id == 1)
		fprintf(stderr, "Failed to initialize mutex with id: {%d}\n", index);
	(void)index;
	if (error_id == 2)
		fprintf(stderr, "Failed to initialize compiling mutex\n");
	else if (error_id == 3)
		fprintf(stderr, "Failed to initialize write mutex\n");
	else if (error_id == 4)
		fprintf(stderr, "Failed to initialize state mutex\n");
	fprintf(stderr, "\033[0m");
}

/**
 * @brief Prints error msg specific to mutexes based on an ID into stderr
 */
void mutex_destroy_errors(int error_id, int index)
{
	fprintf(stderr, "\033[0;31m");
	if (error_id == 1)
		fprintf(stderr, "Failed to destroy mutex with id: {%d}\n", index);
	(void)index;
	if (error_id == 2)
		fprintf(stderr, "Failed to destroy compiling mutex\n");
	else if (error_id == 3)
		fprintf(stderr, "Failed to destroy write mutex\n");
	else if (error_id == 4)
		fprintf(stderr, "Failed to destroy state mutex\n");
	fprintf(stderr, "\033[0m");
}
