/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:15:51 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/25 13:42:49 by masanz-s         ###   ########.fr       */
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
	IDLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	FINISH,
	BURNOUT
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
	int				*compiles_required;
	int				*dongle_cooldown;
	int				*total_coders;

	bool			*burn_out;

	size_t			last_compile;
	size_t			*start_time;

	t_coder_state	state;

	pthread_mutex_t	*l_dongle;
	pthread_mutex_t	*r_dongle;

	pthread_mutex_t *compile_lock;
	pthread_mutex_t *write_lock;
	pthread_mutex_t	*state_lock;
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
	pthread_mutex_t write_lock;
	pthread_mutex_t	state_lock;
}	t_program;

// -------- Initialization ---------
int		program_initializer(char **argv, t_program *prog, pthread_mutex_t **dongles);

// ----------- Clean up ------------
void	clean_values(pthread_t monitor_thread, t_program *prog, pthread_mutex_t *dongles);
void	pthread_mutex_destroy_all(t_program *prog, pthread_mutex_t *dongles);

// ---------- Validation -----------
int		check_argv(int argc, char *argv[]);
int		check_valid_num(char *argv[]);
void	get_rules(char *argv[], t_program *prog);

// ------------ Monitor ------------
void	*monitor(void *pointer);

// ------------- Locks -------------
int		get_burnout_flag(t_coder *coder);
int		get_total_compiles(t_coder *coder);
void	set_state(t_coder *coder, t_coder_state state);
t_coder_state	get_state(t_coder *coder);

// --------- Coder Routine ---------
void	*coder_routine(void *arg);

// ------------ Display ------------
void	display_status(t_coder *coder);
void	display_dongle(t_coder *coder, char *dongle);

// ------------- Utils -------------
int		ft_atoi(const char *nptr);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_itoa(int num);
int		get_program_time(size_t start, size_t current_time);
int		interruptible_sleep(t_coder *coder, int miliseconds);
size_t	ft_strlen(const char *s);
size_t 	get_current_time(void);

// ------------- Errors -------------
void	invalid_num_of_args(void);
void	invalid_num(int error_id, int index, char *value);
void	wrong_scheduler(char *value);
void	thread_errors(int error_id, int index);
void	mutex_init_errors(int error_id, int index);
void	mutex_destroy_errors(int error_id, int index);
void	time_error(int error_id);

#endif