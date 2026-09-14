/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 09:53:56 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_values(int num_coders, t_coder **coders, t_dongle **dongles,
					 pthread_t **threads);

int     main(int argc, char *argv[])
{
    int				rules[7];
    char			*scheduler;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*threads;

	if (check_argv(argc, argv) == -1)
        return (0);
    get_rules(argv, rules, &scheduler);

	if (init_coders(&coders, rules[0]))
		return (1);
	if (init_dongles(&dongles, rules[0]))
	{
		free(coders);
		return (1);
	}

	if (init_threads(&threads, rules))
	{
		int i = 0;

		while(i < rules[0])
			pthread_mutex_destroy(&dongles[i].lock);
		free(coders);
		free(dongles);
		return (1);
	}
	clean_values(rules[0], &coders, &dongles, &threads);

	return (0);
}

void	clean_values(int num_coders, t_coder **coders, t_dongle **dongles,
					 pthread_t **threads)
{
	while(num_coders--)
	{
		pthread_join((*threads)[num_coders], NULL);
		pthread_mutex_destroy(&(*dongles)[num_coders].lock);
	}
	free(*threads);
	free(*dongles);
	free(*coders);
}

void	*print_hello(){
	pthread_t thisThread = pthread_self();
	printf("Current thread ID: %lu\n", (unsigned long)thisThread);
	printf("Hello\n");
	sleep(1);
	return NULL;
}