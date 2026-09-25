/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 09:56:56 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void display_status(t_coder *coder)
{
	int	prog_time;
	int	id;

	prog_time = get_program_time(*(coder->start_time), get_current_time());
	id = (*coder).id;
    if ((*coder).state == COMPILING)
		printf("\033[32m%d %d is compiling\033[0m\n", prog_time, id);
    else if ((*coder).state == DEBUGGING)
		printf("\033[35m%d %d is debugging\033[0m\n", prog_time, id);
    else if ((*coder).state == REFACTORING)
        printf("\033[37m%d %d is refactoring\033[0m\n", prog_time, id);
	else if ((*coder).state == BURNOUT)
        printf("\033[0;31m%d %d burned out\033[0m\n", prog_time, id);
}

void	display_dongle(t_coder *coder, char *dongle)
{
	int	prog_time;
	int	id;

	pthread_mutex_lock((*coder).write_lock);
	if (!*(coder->burn_out))
	{
		prog_time = get_program_time(*(coder->start_time), get_current_time());
		id = (*coder).id;
		printf("\033[30m%d %d has taken %s dongle\033[0m\n",
			prog_time, id, dongle);
	}
	pthread_mutex_unlock((*coder).write_lock);
}

