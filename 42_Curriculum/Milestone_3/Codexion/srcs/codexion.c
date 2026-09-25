/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 12:47:12 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 13:47:12 by masanz-s         ###   ########.fr       */
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
	if (program_initializer(argv, &prog, &dongles))
		return (1);
	return (0);
}
