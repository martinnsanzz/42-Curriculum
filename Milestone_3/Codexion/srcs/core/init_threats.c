/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:56 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 16:47:47 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	init_thread_args(t_thread_arg **args, t_coder **coders,
                             t_dongle **dongles, int *rules);

int	init_threads(t_coder **coders, t_dongle **dongles, pthread_t **threads, int *rules)
{
	int				i;
	t_thread_arg	*args;

	if (init_thread_args(&args, coders, dongles, rules))
		return (1);
	*threads = ft_calloc(rules[0], sizeof(pthread_t));
	if (*threads == NULL)
		return (free(args), 1);
	i = 0;
	while (i < rules[0])
	{
		if (pthread_create(&(*threads)[i], NULL, print_hello, (void *)&args[i]))
		{
			fprintf(stderr, "\033[0;31mFailed to create"
							"thread number: {%d}\n\033[0m", i + 1);
			while (i-- > 0)
				pthread_join((*threads)[i], NULL);
			free(*threads);
			*threads = NULL;
			return (free(args->shared), free(args), 1);
		}
		i++;
	}
	return (free(args->shared), free(args), 0);
}

static int	init_thread_args(t_thread_arg **args, t_coder **coders,
                             t_dongle **dongles, int *rules)
{
	int			i;
	t_shared	*shared;

	init_shared_data(&shared, dongles, rules);
	if (shared == NULL)
		return (1);

	*args = ft_calloc(rules[0], sizeof(t_thread_arg));
    if (*args == NULL)
        return (1);

    i = 0;
    while (i < rules[0])
    {
        (*args)[i].coder = &(*coders)[i];
        (*args)[i].shared = shared;
        i++;
    }
    return (0);
}