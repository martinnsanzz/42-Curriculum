/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:15:51 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/17 14:25:07 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <aio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <limits.h>
# include <stdint.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_thread_arg	t_thread_arg;

// ---------- Enums -----------
typedef enum e_coder_state
{
	INIT,
	COMPILING,
	DEBUGGING,
	REGACTORING,
	BURNOUT,
	FINISH
}	t_coder_state;

// ---------- Structs -----------
typedef struct s_coder
{
	pthread_t		thread;

	int				id;
	int				total_compiles;
	int				time_to_burn_out;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;

	bool			is_burn_out;

	t_coder_state	state;

	pthread_mutex_t	*l_dongle;
	pthread_mutex_t	*r_dongle;

	t_coder_state	coder_state;
} t_coder;

typedef struct s_program
{
	int				compiles_required;
	int				dongle_cooldown;
	int				total_coders;

	char			*scheduler;

	bool			burn_out_flag;

	t_coder			*coders;

}	t_program;



// -------------- Test -------------
void		*print_hello(void *arg);

// -------- Initialization ---------
int			data_initializer(t_program *prog, pthread_mutex_t **dongles);
int			init_threads(t_program *prog);

// ----------- Clean up ------------
void		clean_values(int total_coders, t_coder *coders, pthread_mutex_t *dongles);
void 		pthread_mutex_destroy_all(pthread_mutex_t *dongles, int total_dongles);

// ---------- Validation -----------
int			check_argv(int argc, char *argv[]);
int			check_valid_num(char *argv[]);
void		get_rules(char *argv[], t_program *prog);

// ------------ Display ------------
void		display_status(int timestamp_ms, int coder_id, char *state);

// ------------- Utils -------------
int			ft_atoi(const char *nptr);
int			ft_strcmp(const char *s1, const char *s2);
size_t		ft_strlen(const char *s);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_itoa(int num);

// ------------- Errors -------------
void		invalid_num_of_args();
void		invalid_num(int error_id, int index, char *value);
void		wrong_scheduler(char *value);
void		thread_errors(int error_id, int index);
void		mutex_errors(int error_id, int index);

#endif