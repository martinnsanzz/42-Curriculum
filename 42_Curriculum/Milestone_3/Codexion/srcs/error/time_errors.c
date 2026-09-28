/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 12:10:11 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 11:02:40 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Prints error msg specific if @fn gettimeofday() fails.
 */
void time_error(int error_id)
{
	fprintf(stderr, RED);
	if (error_id == 1)
		fprintf(stderr, "Failed to get time from 'gettimeofday()'\n");
	fprintf(stderr, RESET);
}