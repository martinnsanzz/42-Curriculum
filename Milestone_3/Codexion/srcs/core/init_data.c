/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:54 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 16:43:28 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	init_coders(t_coder **coders, int num_coders)
{
	t_coder_state	state;
	int				i;

	state = INIT;
	*coders = ft_calloc(num_coders, sizeof(t_coder));
	if (*coders == NULL)
		return (1);

	i = 0;
	while(i < num_coders)
	{
		(*coders)[i].coder_id = (i + 1);
		(*coders)[i].total_compiles = 0;
		(*coders)[i].coder_state = state;

		(*coders)[i].right_dongle_i = (i + 1);

		if (i == 0)
			(*coders)[i].left_dongle_i = num_coders;
		else
			(*coders)[i].left_dongle_i = i;
		i++;
	}
	return (0);
}

int	init_dongles(t_dongle **dongles, int num_dongles)
{
	int	i;
	int	error;

	*dongles = ft_calloc(num_dongles, sizeof(t_dongle));
	if (*dongles == NULL)
		return (1);
	i = 0;
	while(i < num_dongles)
	{
		(*dongles)[i].dongle_id = (i + 1);
		(*dongles)[i].dongle_state = AVAILABLE;
		error = pthread_mutex_init(&(*dongles)[i].lock, NULL);
		if (error)
		{
			fprintf(stderr, "\033[0;31mFailed to initialize"
							"mutex number: {%d}\n\033[0m", i + 1);
			while (i-- > 0)
				pthread_mutex_destroy(&(*dongles)[i].lock);
			free(*dongles);
			*dongles = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int init_shared_data(t_shared **shared, t_dongle **dongles, int *rules)
{
	*shared = ft_calloc(rules[0], sizeof(t_shared));
	if (*shared == NULL)
		return (1);

	(*shared)->dongles = *dongles;
	(*shared)->rules = rules;

	return (0);
}