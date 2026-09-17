/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 14:25:34 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int     main(int argc, char *argv[])
{
    t_program		prog;
	pthread_mutex_t	*dongles;

	if (check_argv(argc, argv) == 1)
        return (1);

	prog.total_coders = ft_atoi(argv[1]);
	if (data_initializer(&prog, &dongles))
		return (1);

	get_rules(argv, &prog);
	if (init_threads(&prog))
	{
		pthread_mutex_destroy_all(dongles, prog.total_coders);
		return (1);
	}
    clean_values(prog.total_coders, prog.coders, dongles);
	return (0);
}

void	*print_hello(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;

	printf("Coder id: %d\n", (*coder).id);
	sleep(1);
	return NULL;
}