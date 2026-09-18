/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:36:24 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/18 15:25:22 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../codexion.h"

static int	check_total_compiles(t_program *prog);
static int	check_burnouts(t_program *prog);
static int	check_coder_burn(t_program *prog, t_coder coder);

void	*monitor(void *pointer)
{
	t_program *prog;

	prog = (t_program *)pointer;
	while (1)
	{
		if (check_total_compiles(prog) || check_burnouts(prog))
			break ;
	}
	return (pointer);
}


static int	check_burnouts(t_program *prog)
{
	int		i;

	i = 0;
	while (i < (*prog).total_coders)
	{
		if ((*prog).coders[i].state != COMPILING)
		{
			if (check_coder_burn(prog, (*prog).coders[i]))
			{
				pthread_mutex_lock(&(*prog).burnout_lock);
				(*prog).burn_out_flag = true;
				pthread_mutex_unlock(&(*prog).burnout_lock);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int check_coder_burn(t_program *prog, t_coder coder)
{
	size_t	last_compile;
	size_t	current_time;

	last_compile = coder.last_compile;
	current_time = get_current_time();
	if ((int)(current_time - last_compile) > coder.time_to_burn_out)
	{
		pthread_mutex_lock(&(*prog).write_lock);
		display_status((*prog).start_time, coder.id, BURNOUT);
		pthread_mutex_unlock(&(*prog).write_lock);
		return (1);
	}
	return (0);
}
static int check_total_compiles(t_program *prog)
{
	int	i;
	int	total_compiles;

	i = 0;
	total_compiles = 0;
	while (i < (*prog).total_coders)
	{
		if ((*prog).coders[i].total_compiles >= (*prog).compiles_required)
			total_compiles += 1;
		i++;
	}
	if (total_compiles == (*prog).total_coders)
		return (1);
	return (0);
}
