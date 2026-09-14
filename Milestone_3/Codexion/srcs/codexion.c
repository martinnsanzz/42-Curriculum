/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 12:31:46 by masanz-s         ###   ########.fr       */
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

	if (check_argv(argc, argv) == 1)
        return (1);
    get_rules(argv, rules, &scheduler);

	if (initializer(&coders, &dongles, &threads, rules))
		return (1);
	clean_values(rules[0], &coders, &dongles, &threads);

	return (0);
}

void	clean_values(int num_coders, t_coder **coders, t_dongle **dongles,
					 pthread_t **threads)
{
	int	i;
	int	error;

	i = 0;
	while(i < num_coders)
	{
		error = pthread_join((*threads)[i], NULL);
		if (error){
			fprintf(stderr, "\033[0;31mFailed to join "
							"thread number: {%d}\n\033[0m", i + 1);
			}
		error = pthread_mutex_destroy(&(*dongles)[i].lock);
		if (error){
			fprintf(stderr, "\033[0;31mFailed to destroy "
							"dongle lock number: {%d}\n\033[0m", i + 1);
			}
		i++;
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