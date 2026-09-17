/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:36:24 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 16:34:15 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../codexion.h"

static int	check_total_compiles(t_program *prog);
static int	check_burnout(t_program *prog);

void	*monitor(void *pointer)
{
	t_program *prog;

	prog = (t_program *)pointer;
	while (1)
	{
		if (check_total_compiles(prog) || check_burnout(prog))
			break ;
	}
	return (pointer);
}

static int	check_burnout(t_program *prog)
{
	(void)prog;
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
