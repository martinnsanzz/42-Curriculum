/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 14:23:23 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/22 13:31:23 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	compile(t_coder *coder);
static int	compile_helper(t_coder *coder);
static int	debugging(t_coder *coder);
static int	refactor(t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	while(!*(coder->burn_out) && coder->state != FINISH)
	{
		if (compile(coder))
			break ;
		if (debugging(coder))
			break ;
		if (refactor(coder))
			break ;
		if (coder->total_compiles == coder->compiles_required)
			coder->state = FINISH;
	}
	return arg;
}

static int compile(t_coder *coder)
{
	pthread_mutex_t *first_dongle;
	pthread_mutex_t *second_dongle;

	if ((*coder).id % 2 == 0)
	{
		first_dongle = coder->l_dongle;
		second_dongle = coder->r_dongle;
	}
	else
	{
		first_dongle = coder->r_dongle;
		second_dongle = coder->l_dongle;
	}
	pthread_mutex_lock(first_dongle);
	display_dongle(*(coder->start_time), coder, "left");
	if (*(*coder).total_coders == 1)
	{
		interruptible_sleep(coder, coder->time_to_burn_out);
		(*coder).state = FINISH;
		return (pthread_mutex_unlock(first_dongle), 1);
	}
	pthread_mutex_lock(second_dongle);
	display_dongle(*(coder->start_time), coder, "right");
	if(compile_helper(coder))
	{
		pthread_mutex_unlock(first_dongle);
		pthread_mutex_unlock(second_dongle);
		return (1);
	}
	pthread_mutex_unlock(first_dongle);
	pthread_mutex_unlock(second_dongle);
	(*coder).state = IDLE;
	return (0);
}

static int	compile_helper(t_coder *coder)
{
	int	error;

	pthread_mutex_lock((*coder).compile_lock);
	(*coder).state = COMPILING;
	error = interruptible_sleep(coder, coder->time_to_compile);
	if (!error)
	{
		display_status(*(coder->start_time), coder);
		(*coder).last_compile = get_current_time();
		(*coder).total_compiles += 1;
	}
	pthread_mutex_unlock((*coder).compile_lock);
	return (error);
}
static int debugging(t_coder *coder)
{
	(*coder).state = DEBUGGING;
	display_status(*(coder->start_time), coder);
	if (interruptible_sleep(coder, coder->time_to_debug))
		return (1);
	(*coder).state = IDLE;
	return (0);
}

static int refactor(t_coder *coder)
{
	(*coder).state = REFACTORING;
	display_status(*(coder->start_time), coder);
	if (interruptible_sleep(coder, coder->time_to_refactor))
		return (1);
	(*coder).state = IDLE;
	return (0);
}
