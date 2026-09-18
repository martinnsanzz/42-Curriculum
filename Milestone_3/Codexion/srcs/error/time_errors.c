/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 12:10:11 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/18 12:54:01 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Prints error msg specific to time of the program
 */
void time_error(int error_id)
{
	fprintf(stderr, "\033[0;31m");
	if (error_id == 1)
		fprintf(stderr, "Failed to get time from 'gettimeofday()'\n");
	fprintf(stderr, "\033[0m");
}