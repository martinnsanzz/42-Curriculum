/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:56 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/15 12:43:57 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int create_thread_args(t_program *prog);
static int create_threads(t_program *prog);

int	init_threads(t_program *prog)
{
	int	failed_at;
    int	i;

	if (create_thread_args(prog))
		return (1);
	(*prog).threads = ft_calloc((*prog).rules[0], sizeof(pthread_t));
	
    if ((*prog).threads == NULL)
		return (free((*prog).args), 1);
	
    failed_at = create_threads(prog);
	if (failed_at < 0)
	{

		i = -failed_at - 1;
		while (i-- > 0)
			pthread_join((*prog).threads[i], NULL);
		free((*prog).threads);
		(*prog).threads = NULL;
		free((*prog).args);
		(*prog).args = NULL;
		return (1);
	}
	return (0);
}

static int create_threads(t_program *prog)
{
	int	i;
	int	error;

	i = 0;
	while (i < (*prog).rules[0])
	{
		error = pthread_create(&(*prog).threads[i], NULL, print_hello,
				(void *)&(*prog).args[i]);
		if (error)
			return (-(i + 1));
		i++;
	}
	return (0);
}

static int	create_thread_args(t_program *prog)
{
	int i;

    (*prog).args = ft_calloc((*prog).rules[0], sizeof(t_thread_arg));
    if ((*prog).args == NULL)
        return (1);
    
    i = 0;
    while (i < (*prog).rules[0])
    {
        (*prog).args[i].coder = &(*prog).coders[i];
        (*prog).args[i].prog = &(*prog);
        i++;
    }
    return (0);
}
