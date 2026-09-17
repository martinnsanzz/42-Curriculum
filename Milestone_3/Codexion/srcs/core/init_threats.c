/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:56 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 14:21:15 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

/**
 * @brief Initializes and creates all threads of the program.
 *
 * Creates one thread per coder, each running @c print_hello with a
 * pointer to its own @c t_coder as argument. If a thread fails to
 * create partway through, it joins every thread already created so
 * far, frees the coders array, and returns — the caller never works
 * with a partially-created set of threads.
 *
 * @param prog Pointer to the program struct; its @c coders array is
 *             read from (each coder's thread field is written into),
 *             and freed on failure.
 *
 * @return 0 on success (every thread created).
 * @return 1 on failure (thread creation failed at some index);
 *         @p prog->coders is freed and set to NULL.
 */
int	init_threads(t_program *prog)
{
	int	i;
	int error;

	i = 0;
	while (i < (*prog).total_coders)
	{
		error = pthread_create(&(*prog).coders[i].thread, NULL, print_hello,
				(void *)&(*prog).coders[i]);
		if (error)
		{
			thread_errors(1, i + 1);
			while (i-- > 0)
				pthread_join((*prog).coders[i].thread, NULL);
			free((*prog).coders);
			(*prog).coders = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
