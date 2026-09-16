/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:40:56 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 15:08:42 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int create_thread_args(t_program *prog);
static int create_threads(t_program *prog);

/**
 * @brief Initializes and creates all threads of the program.
 *
 * It allocates all respective args struct for each thread. Allocates
 * each `pthread_t` struct and creates the threads running the respective
 * function. If anything fails mid-way it safely joins and frees any previously
 * created thread so the user never works with partially initialized threads.
 *
 * @return 0 on success (Allocation succesfull).
 * @return -(i + 1) on failure; The index of what thread failed + 1 so index 0
 * 		   is not mixed up as a success and - value for easier cleanup method later
 * 		   on.
 */
int	init_threads(t_program *prog)
{
	if (create_thread_args(prog))
		return (1);

	(*prog).threads = ft_calloc((*prog).rules[0], sizeof(pthread_t));
    if ((*prog).threads == NULL)
		return (free((*prog).args), 1);

	if (create_threads(prog))
		return (1);
	return (0);
}

/**
 * @brief Creates a thread for each coder.
 *
 * It creates a thread with a function to call in parallel with the respective
 * args struct.
 *
 * @param prog Pointer to the program struct to initialize.
 *
 * @return 0 on success (Allocation succesfull).
 * @return -(i + 1) on failure; The index of what thread failed + 1 so index 0
 * 		   is not mixed up as a success and - value for easier cleanup method later
 * 		   on.
 */
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
		{
			thread_errors(1, i + 1);
			while (i-- > 0)
				pthread_join((*prog).threads[i], NULL);
			free((*prog).threads);
			(*prog).threads = NULL;
			free((*prog).args);
			(*prog).args = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Creates and allocates an args struct to pass to a function.
 *
 * For each coder a struct is made with the coder struct and the @p prog
 * information. Each args member also contains @p prog.coders array but
 * @p prog.args[i].coder is made for easier access for future use in the
 * program.
 *
 * @param prog Pointer to the program struct to initialize.
 *
 * @return 0 on success (Allocation succesfull).
 * @return 1 on failure.
 */
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
