/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 14:23:23 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 13:50:35 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	compile(t_coder *coder);
static int	debugging(t_coder *coder);
static int	refactor(t_coder *coder);
static int	compile_helper(t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (*(coder->compiles_required) == 0)
	{
		set_state(coder, FINISH);
		return (NULL);
	}
	if (*(*coder).total_coders == 1)
	{
		pthread_mutex_lock(coder->l_dongle);
		display_dongle(coder, "left");
		interruptible_sleep(coder, coder->time_to_burn_out);
		set_state(coder, BURNOUT);
		display_status(coder);
		return (pthread_mutex_unlock(coder->l_dongle), NULL);
	}
	while (!*coder->burn_out && get_state(coder) != FINISH)
	{
		if (compile(coder) || debugging(coder) || refactor(coder))
			break ;
		if (get_total_compiles(coder) == *(coder->compiles_required))
			set_state(coder, FINISH);
	}
	return (arg);
}

static int compile(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(coder->l_dongle);
		display_dongle(coder, "left");
		pthread_mutex_lock(coder->r_dongle);
		display_dongle(coder, "right");
	}
	else
	{
		pthread_mutex_lock(coder->r_dongle);
		display_dongle(coder, "right");
		pthread_mutex_lock(coder->l_dongle);
		display_dongle(coder, "left");
	}
	if (compile_helper(coder))
		return (1);
	set_state(coder, IDLE);
	return (0);
}

static int	compile_helper(t_coder *coder)
{
{
	int	error;
	int	burnt;

	pthread_mutex_lock((*coder).write_lock);
	burnt = *(*coder).burn_out;
	if (!burnt)
	{
		set_state(coder, COMPILING);
		display_status(coder);
	}
	pthread_mutex_unlock((*coder).write_lock);
	if (burnt)
	{
		pthread_mutex_unlock((*coder).l_dongle);
		return (pthread_mutex_unlock((*coder).r_dongle), 1);
	}
	pthread_mutex_lock((*coder).compile_lock);
	(*coder).last_compile = get_current_time();
	(*coder).total_compiles += 1;
	pthread_mutex_unlock((*coder).compile_lock);
	error = interruptible_sleep(coder, coder->time_to_compile);
	pthread_mutex_unlock((*coder).l_dongle);
	return (pthread_mutex_unlock((*coder).r_dongle), error);
}
}

static int debugging(t_coder *coder)
{
	int	burnt;

	pthread_mutex_lock((*coder).write_lock);
	burnt = *(*coder).burn_out;
	if (!burnt)
	{
		set_state(coder, DEBUGGING);
		display_status(coder);
	}
	pthread_mutex_unlock((*coder).write_lock);
	if (burnt)
		return (1);
	if (interruptible_sleep(coder, coder->time_to_debug))
		return (1);
	set_state(coder, IDLE);
	return (0);
}

static int refactor(t_coder *coder)
{
	int	burnt;

	pthread_mutex_lock((*coder).write_lock);
	burnt = *(*coder).burn_out;
	if (!burnt)
	{
		set_state(coder, REFACTORING);
		display_status(coder);
	}
	pthread_mutex_unlock((*coder).write_lock);
	if (burnt)
		return (1);
	if (interruptible_sleep(coder, coder->time_to_refactor))
		return (1);
	set_state(coder, IDLE);
	return (0);
}
