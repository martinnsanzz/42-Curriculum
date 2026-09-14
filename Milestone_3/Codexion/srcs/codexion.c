/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 16:52:37 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int     main(int argc, char *argv[])
{
    int				rules[7];
    char			*scheduler;

	if (check_argv(argc, argv) == 1)
        return (1);
    get_rules(argv, rules, &scheduler);

	if (data_initializer(rules))
		return (1);

	return (0);
}

int	data_initializer(int *rules)
{
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*threads;

	if (init_coders(&coders, rules[0]))
		return (1);

	if (init_dongles(&dongles, rules[0]))
		return (free (coders), 1);

	if (init_threads(&coders, &dongles, &threads, rules))
	{
		while(rules[0]--)
			pthread_mutex_destroy(&dongles[rules[0]].lock);
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
	int	i;
	int	error;

	i = 0;
	while(i < num_coders)
	{
		error = pthread_join((*threads)[i], NULL);
		if (error)
			fprintf(stderr, "\033[0;31mFailed to join thread number: {%d}\n\033[0m", i + 1);
		error = pthread_mutex_destroy(&(*dongles)[i].lock);
		if (error)
			fprintf(stderr, "\033[0;31mFailed to destroy lock number: {%d}\n\033[0m", i + 1);
		i++;
	}
	free(*threads);
	free(*dongles);
	free(*coders);
}

void	*print_hello(void *arg)
{
	t_thread_arg *data;
	int coder_id;

	data = (t_thread_arg *)arg;
	coder_id = data->coder->coder_id;

	pthread_t thisThread = pthread_self();
	printf("This thread id: %lu\n", thisThread);

	printf("Coder id: %d\n", coder_id);
	printf("Coder %d left dongle: %d\n", coder_id, data->coder->left_dongle_i);
	printf("Coder %d right dongle: %d\n", coder_id, data->coder->right_dongle_i);
	sleep(1);
	return NULL;
}