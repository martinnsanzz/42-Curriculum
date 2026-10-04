/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 12:10:11 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:14:27 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Prints error msg specific if @fn gettimeofday() fails.
 */
void	time_error(int error_id)
{
	fprintf(stderr, RED);
	if (error_id == 1)
		fprintf(stderr, "Failed to get time from 'gettimeofday()'\n");
	fprintf(stderr, RESET);
}
