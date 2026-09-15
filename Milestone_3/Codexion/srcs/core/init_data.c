/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:54 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/15 12:44:21 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int  init_coders(t_program *prog);
static int  init_dongles(t_program *prog);

int	data_initializer(t_program *program)
{
    int i;

	if (init_coders(program))
		return (1);

	if (init_dongles(program))
		return (free ((*program).coders), 1);

	if (init_threads(program))
	{
        i = 0;
		while(i < (*program).rules[0])
			pthread_mutex_destroy(&(*program).dongles[i].lock);
		free((*program).coders);
		free((*program).dongles);
		return (1);
	}
	return (0);
}

static int  init_coders(t_program *prog)
{
    t_coder_state	state;
	int				i;

	state = INIT;
	(*prog).coders = ft_calloc((*prog).rules[0], sizeof(t_coder));
	if ((*prog).coders == NULL)
		return (1);

	i = 0;
	while(i < (*prog).rules[0])
	{
		(*prog).coders[i].coder_id = (i + 1);
		(*prog).coders[i].total_compiles = 0;
		(*prog).coders[i].coder_state = state;

		(*prog).coders[i].right_dongle_i = (i + 1);

		if (i == 0)
			(*prog).coders[i].left_dongle_i = (*prog).rules[0];
		else
			(*prog).coders[i].left_dongle_i = i;
		i++;
	}
	return (0);
}

static int  init_dongles(t_program *prog)
{
    int	i;
	int	error;

	(*prog).dongles = ft_calloc((*prog).rules[0], sizeof(t_dongle));
	if ((*prog).dongles == NULL)
		return (1);
	i = 0;
	while(i < (*prog).rules[0])
	{
		(*prog).dongles[i].dongle_id = (i + 1);
		(*prog).dongles[i].dongle_state = AVAILABLE;
		error = pthread_mutex_init(&(*prog).dongles[i].lock, NULL);
		if (error)
		{
			fprintf(stderr, "\033[0;31mFailed to initialize"
							"mutex number: {%d}\n\033[0m", i + 1);
			while (i-- > 0)
				pthread_mutex_destroy(&(*prog).dongles[i].lock);
			free((*prog).dongles);
			(*prog).dongles = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
