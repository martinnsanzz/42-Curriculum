/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 14:54:35 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	data_initializer(t_program *program);


int     main(int argc, char *argv[])
{
    t_program program;

	if (check_argv(argc, argv) == 1)
        return (1);
    get_rules(argv, &program);

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
			thread_errors(2, i + 1);
		error = pthread_mutex_destroy(&(*program).dongles[i].lock);
		if (error)
			mutex_errors(2, i + 1);
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
	sleep(1);
	return NULL;
}