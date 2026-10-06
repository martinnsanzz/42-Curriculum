/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 13:54:43 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:14:17 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Prints error msg specific to threads based on an ID into stderr
 */
void	thread_errors(int error_id, int index)
{
	fprintf(stderr, RED);
	if (error_id == 1)
		fprintf(stderr, "Failed to create thread number: {%d}\n", index);
	else if (error_id == 2)
		fprintf(stderr, "Failed to join thread number: {%d}\n", index);
	(void)index;
	if (error_id == 3)
		fprintf(stderr, "Failed to create monitor thread\n");
	else if (error_id == 4)
		fprintf(stderr, "Failed to join monitor thread\n");
	if (error_id == 5)
		fprintf(stderr, "Failed to create scheduler thread\n");
	else if (error_id == 6)
		fprintf(stderr, "Failed to join scheduler thread\n");
	fprintf(stderr, RESET);
}

/**
 * @brief Prints error msg specific to mutexes based on an ID into stderr
 */
void	mutex_init_errors(int error_id, int index)
{
	fprintf(stderr, RED);
	if (error_id == 1)
		fprintf(stderr, "Failed to initialize dongle mutex: {%d}\n", index);
	(void)index;
	if (error_id == 2)
		fprintf(stderr, "Failed to initialize compiling mutex\n");
	else if (error_id == 3)
		fprintf(stderr, "Failed to initialize write mutex\n");
	else if (error_id == 4)
		fprintf(stderr, "Failed to initialize state mutex\n");
	else if (error_id == 5)
		fprintf(stderr, "Failed to initialize priority mutex\n");
	fprintf(stderr, RESET);
}

/**
 * @brief Prints error msg specific to mutexes based on an ID into stderr
 */
void	mutex_destroy_errors(int error_id, int index)
{
	fprintf(stderr, RED);
	if (error_id == 1)
		fprintf(stderr, "Failed to destroy mutex with id: {%d}\n", index);
	(void)index;
	if (error_id == 2)
		fprintf(stderr, "Failed to destroy compiling mutex\n");
	else if (error_id == 3)
		fprintf(stderr, "Failed to destroy write mutex\n");
	else if (error_id == 4)
		fprintf(stderr, "Failed to destroy state mutex\n");
	else if (error_id == 5)
		fprintf(stderr, "Failed to destroy priority mutex\n");
	fprintf(stderr, RESET);
}

/**
 * @brief Prints error msg specific to cond variables based on an ID into stderr
 */
void	cond_erors(int error_id)
{
	fprintf(stderr, RED);
	if (error_id == 1)
		fprintf(stderr, "Failed to initialize turn condition.\n");
	else if (error_id == 2)
		fprintf(stderr, "Failed to destroy turn condition.\n");
	fprintf(stderr, RESET);
}
