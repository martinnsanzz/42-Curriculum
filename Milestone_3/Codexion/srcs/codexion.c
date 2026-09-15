/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/15 12:46:51 by 2002mssm02       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	data_initializer(t_program *program);

int     main(int argc, char *argv[])
{
    t_program program;

	if (check_argv(argc, argv) == 1)
        return (1);
    get_rules(argv, program.rules, &program.scheduler);

	if (data_initializer(&program))
		return (1);
    clean_values(&program);
	return (0);
}

void	clean_values(t_program *program)
{
	int	i;
	int	error;

	i = 0;
	while(i < (*program).rules[0])
	{
		error = pthread_join((*program).threads[i], NULL);
		if (error)
			fprintf(stderr, "\033[0;31mFailed to join thread number: {%d}\n\033[0m", i + 1);
		error = pthread_mutex_destroy(&(*program).dongles[i].lock);
		if (error)
			fprintf(stderr, "\033[0;31mFailed to destroy lock number: {%d}\n\033[0m", i + 1);
		i++;
	}
    free((*program).args);
	free((*program).threads);
	free((*program).coders);
	free((*program).dongles);
}

void	*print_hello(void *arg)
{
	t_thread_arg *data;
	int coder_id;

	data = (t_thread_arg *)arg;
	coder_id = data->coder->coder_id;


	printf("Coder id: %d\n", coder_id);
	printf("Coder %d left dongle: %d\n", coder_id, data->coder->left_dongle_i);
	printf("Coder %d right dongle: %d\n", coder_id, data->coder->right_dongle_i);
	sleep(5);
	return NULL;
}