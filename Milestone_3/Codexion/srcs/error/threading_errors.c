/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:54:43 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 17:01:43 by masanz-s         ###   ########.fr       */
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
	else if (error_id == 3)
	{
		(void)index;
		fprintf(stderr, "Failed to create monitor thread\n");
	}
	else if (error_id == 4)
	{
		(void)index;
		fprintf(stderr, "Failed to join monitor thread\n");
	}

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
	else if (error_id == 2)
	{
		(void)index;
		fprintf(stderr, "Failed to initialize compiling mutex\n");
	}
	else if (error_id == 3)
	{
		(void)index;
		fprintf(stderr, "Failed to initialize finish mutex\n");
	}
	else if (error_id == 4)
	{
		(void)index;
		fprintf(stderr, "Failed to initialize burnout mutex\n");
	}

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
	else if (error_id == 2)
	{
		(void)index;
		fprintf(stderr, "Failed to destroy compiling mutex\n");
	}
	else if (error_id == 3)
	{
		(void)index;
		fprintf(stderr, "Failed to destroy finish mutex\n");
	}
	else if (error_id == 4)
	{
		(void)index;
		fprintf(stderr, "Failed to destroy burnout mutex\n");
	}
	
	fprintf(stderr, "\033[0m");
}
