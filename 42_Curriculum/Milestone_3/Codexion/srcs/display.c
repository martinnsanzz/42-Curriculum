/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 14:29:51 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void display_status(t_coder *coder)
{
	int		id;
	size_t	prog_time;

	prog_time = get_program_time(*(coder->start_time));
	id = (*coder).id;
    if ((*coder).state == COMPILING)
		printf(LOG_COMPILING, GREEN, prog_time, id);
    else if ((*coder).state == DEBUGGING)
		printf(LOG_DEBUGGING, PURPLE, prog_time, id);
    else if ((*coder).state == REFACTORING)
        printf(LOG_REFACTOR, WHITE, prog_time, id);
	else if ((*coder).state == BURNOUT)
        printf(LOG_BURNS_OUT, RED, prog_time, id);
}

void	display_dongle(t_coder *coder, char *dongle)
{
	int		id;
	size_t	prog_time;

	pthread_mutex_lock((*coder).write_lock);
	if (!*(coder->burn_out))
	{
		prog_time = get_program_time(*(coder->start_time));
		id = (*coder).id;
		printf(LOG_TAKE_DONGLE, BLACK, prog_time, id, dongle);
	}
	pthread_mutex_unlock((*coder).write_lock);
}

