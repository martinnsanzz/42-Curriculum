/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduling.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 13:22:37 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/30 14:53:06 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	*sched_routine(void *pointer)
{
	t_schedule *sched;
	int total_cod;
	int i;
	sched = (t_schedule *)pointer;

	total_cod = *(sched->coders[0]->total_coders);
	i = 0;
	while (i < total_cod)
	{
		i++;
	}
	return (NULL);
}

// static int fifo(t_schedule )