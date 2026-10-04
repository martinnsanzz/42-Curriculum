/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/04 19:31:59 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	display_status(t_coder *coder)
{
	int		id;
	size_t	prog_time;

	prog_time = get_program_time(*(coder->start_time));
	id = (*coder).id;
	if ((*coder).state == COMPILING)
		printf(LOG_COMPILING, GREEN, prog_time, id, RESET);
	else if ((*coder).state == DEBUGGING)
		printf(LOG_DEBUGGING, PURPLE, prog_time, id, RESET);
	else if ((*coder).state == REFACTORING)
		printf(LOG_REFACTOR, WHITE, prog_time, id, RESET);
	else if ((*coder).state == BURNOUT)
		printf(LOG_BURNS_OUT, RED, prog_time, id, RESET);
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
		printf(LOG_TAKE_DONGLE, BLACK, prog_time, id, dongle, RESET);
	}
	pthread_mutex_unlock((*coder).write_lock);
}
