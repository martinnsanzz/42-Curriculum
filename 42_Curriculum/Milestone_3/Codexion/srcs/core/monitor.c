/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:36:24 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/24 16:36:48 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../codexion.h"

static int	check_total_compiles(t_program *prog);
static int	check_burnouts(t_program *prog);

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
	size_t	last_compile;
	size_t	current_time;
	int	i;

	i = 0;
	while (i < (*prog).total_coders)
	{
		last_compile = (*prog).coders[i].last_compile;
		current_time = get_current_time();
		if ((*prog).coders[i].state != FINISH
			&& (*prog).coders[i].state != COMPILING)
		{
			if ((int)(current_time - last_compile) > (*prog).coders[i].time_to_burn_out)
			{
				(*prog).burn_out_flag = true;
				set_state(&(*prog).coders[i], BURNOUT);
				return (display_status(&(*prog).coders[i]), 1);
			}
		}
		i++;
	}
	return (0);
}

static int check_total_compiles(t_program *prog)
{
	int	i;
	int	finished_coders;

	i = 0;
	finished_coders = 0;
	while (i < (*prog).total_coders)
	{
		if (get_state(&(*prog).coders[i]) == FINISH)
			finished_coders += 1;
		i++;
	}
	if (finished_coders == (*prog).total_coders)
		return (1);
	return (0);
}
