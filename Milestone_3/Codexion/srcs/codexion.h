/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:15:51 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/18 09:38:32 by masanz-s         ###   ########.fr       */
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

	bool			*burn_out;

	size_t			*start_time;

	t_coder_state	state;

	pthread_mutex_t	*l_dongle;
	pthread_mutex_t	*r_dongle;

	pthread_mutex_t *compile_lock;
	pthread_mutex_t *finish_lock;
	pthread_mutex_t	*burnout_lock;
} t_coder;

typedef struct s_program
{
	int				compiles_required;
	int				dongle_cooldown;
	int				total_coders;

	size_t			start_time;

	char			*scheduler;

	bool			burn_out_flag;

	t_coder			*coders;

	pthread_mutex_t compile_lock;
	pthread_mutex_t finish_lock;
	pthread_mutex_t	burnout_lock;

}	t_program;



// -------------- Test -------------
void		*print_hello(void *arg);

// -------- Initialization ---------
int			program_initializer(char **argv, t_program *prog, pthread_mutex_t **dongles);

// ----------- Clean up ------------
void		clean_values(pthread_t monitor_thread, t_program *prog, pthread_mutex_t *dongles);
void		pthread_mutex_destroy_all(t_program *prog, pthread_mutex_t *dongles);

// ---------- Validation -----------
int			check_argv(int argc, char *argv[]);
int			check_valid_num(char *argv[]);
void		get_rules(char *argv[], t_program *prog);

// ------------ Monitor ------------
void		*monitor(void *pointer);

// ------------ Display ------------
void		display_status(int timestamp_ms, int coder_id, char *state);

// ------------- Utils -------------
int			ft_atoi(const char *nptr);
int			ft_strcmp(const char *s1, const char *s2);
void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_itoa(int num);
size_t		ft_strlen(const char *s);
size_t 		get_current_time();

// ------------- Errors -------------
void		invalid_num_of_args();
void		invalid_num(int error_id, int index, char *value);
void		wrong_scheduler(char *value);
void		thread_errors(int error_id, int index);
void		mutex_init_errors(int error_id, int index);
void 		mutex_destroy_errors(int error_id, int index);

#endif