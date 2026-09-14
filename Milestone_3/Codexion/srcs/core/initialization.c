/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 15:16:32 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 12:38:29 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"


static int	init_coders(t_coder **coders, int num_coders);
static int	init_dongles(t_dongle **dongles, int num_dongles);
static int	init_threads(pthread_t **threads, int *rules);

int	initializer(t_coder **coders, t_dongle **dongles,
				pthread_t **threads, int *rules)
{
	int	i;

	if (init_coders(coders, rules[0]))
		return (1);
	if (init_dongles(dongles, rules[0]))
	{
		free(*coders);
		return (1);
	}

	if (init_threads(threads, rules))
	{
		i = 0;
		while(i < rules[0])
			pthread_mutex_destroy(&(*dongles)[i++].lock);
		free(*coders);
		free(*dongles);
		return (1);
	}
	return (0);
}

static int	init_coders(t_coder **coders, int num_coders)
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

static int	init_dongles(t_dongle **dongles, int num_dongles)
{
	int	i;
	int	error;

	*dongles = ft_calloc(num_dongles, sizeof(t_dongle));
	if (*dongles == NULL)
		return (1);

	i = 0;
	while(i < num_dongles){
		(*dongles)[i].dongle_id = (i + 1);
		(*dongles)[i].dongle_state = AVAILABLE;

		error = pthread_mutex_init(&(*dongles)[i].lock, NULL);
		if (error){
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

static int	init_threads(pthread_t **threads, int *rules)
{
	int	i;
	int	error;

	*threads = ft_calloc(rules[0], sizeof(pthread_t));
	if (*threads == NULL)
		return (1);

	i = 0;
	while(i < rules[0]){
		error = pthread_create(&(*threads)[i], NULL, print_hello, NULL);

		if (error){
			fprintf(stderr, "\033[0;31mFailed to create"
							"thread number: {%d}\n\033[0m", i + 1);
			while (i-- > 0)
				pthread_join((*threads)[i], NULL);
			free(*threads);
			*threads = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
