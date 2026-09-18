/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 14:23:23 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/18 15:56:17 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int compile(t_coder *coder);
static int debugging(t_coder *coder);
static int refactor(t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	while(!*(coder->burn_out) && (coder->total_compiles != coder->compiles_required))
	{
		if (compile(coder))
			break ;
		if (debugging(coder))
			break ;
		if (refactor(coder))
			break ;
	}
	return arg;
}

static int compile(t_coder *coder)
{
	(*coder).state = COMPILING;
	pthread_mutex_lock((*coder).write_lock);
	display_status(*(coder->start_time), coder->id, COMPILING);
	pthread_mutex_unlock((*coder).write_lock);
	(*coder).total_compiles += 1;
	if(interruptible_sleep(coder, coder->time_to_compile))
		return (1);
	return (0);
}

static int debugging(t_coder *coder)
{
	(*coder).state = DEBUGGING;
	pthread_mutex_lock((*coder).write_lock);
	display_status(*(coder->start_time), coder->id, DEBUGGING);
	pthread_mutex_unlock((*coder).write_lock);
	if (interruptible_sleep(coder, coder->time_to_refactor))
		return (1);
	return (0);
}

static int refactor(t_coder *coder)
{
	(*coder).state = REFACTORING;
	pthread_mutex_lock((*coder).write_lock);
	display_status(*(coder->start_time), coder->id, REFACTORING);
	pthread_mutex_unlock((*coder).write_lock);
	if (interruptible_sleep(coder, coder->time_to_refactor))
		return (1);
	return (0);
}
