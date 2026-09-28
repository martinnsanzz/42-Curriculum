/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:15:51 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/28 14:56:17 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

// -------------------- //
//		INCLUDES		//
// -------------------- //

# include <aio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <limits.h>
# include <stdint.h>
# include <pthread.h>
# include <stdbool.h>

// -------------------- //
//		MACROS			//
// -------------------- //

// Colors //
# define BLACK				"\033[30m"
# define RED				"\033[31m"
# define GREEN				"\033[32m"
# define YELLOW				"\033[33m"
# define PURPLE				"\033[35m"
# define WHITE				"\033[37m"
# define RESET				"\033[0m"

// Logs message //
# define LOG_TAKE_DONGLE	"%s[%lu] Coder %d has taken %s dongle\n"
# define LOG_COMPILING		"%s[%lu] Coder %d is compiling\n"
# define LOG_DEBUGGING		"%s[%lu] Coder %d is debugging\n"
# define LOG_REFACTOR		"%s[%lu] Coder %d is refactoring\n"
# define LOG_BURNS_OUT		"%s[%lu] Coder %d burned out\n"
# define LOG_SUCCESS		"\n%sAll coders have compiled. They can rest... for now%s\n"

// Coders limits //
# define MIN_CODERS 	1
# define MAX_CODERS 	200

// Scheduler //
# define FIFO			"fifo"
# define EDF			"edf"

// -------------------- //
//		   ENUM			//
// -------------------- //
typedef enum e_coder_state
{
	IDLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	FINISH,
	BURNOUT
}	t_coder_state;

// -------------------- //
//		STRUCTURES		//
// -------------------- //
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_program	t_program;

typedef struct s_dongle
{
	size_t			last_release;

	pthread_mutex_t	lock;

	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread;

	int				id;
	int				total_compiles;
	int				*compiles_required;
	int				*total_coders;

	bool			*burn_out;

	size_t			time_to_burn_out;
	size_t			time_to_compile;
	size_t			time_to_debug;
	size_t			time_to_refactor;
	size_t			last_compile;
	size_t			*start_time;
	size_t			*dongle_cooldown;

	t_coder_state	state;

	t_dongle		*l_dongle;
	t_dongle		*r_dongle;

	pthread_mutex_t *compile_lock;
	pthread_mutex_t *write_lock;
	pthread_mutex_t	*state_lock;
}	t_coder;

typedef struct s_program
{
	int				compiles_required;
	int				total_coders;

	size_t			dongle_cooldown;
	size_t			start_time;

	char			*scheduler;

	bool			burn_out_flag;

	t_coder			*coders;

	pthread_mutex_t compile_lock;
	pthread_mutex_t write_lock;
	pthread_mutex_t	state_lock;
}	t_program;

// -------------------- //
//		PROTOTYPES		//
// -------------------- //

// ----------- Parsing -------------
int		check_argv(int argc, char *argv[]);
int		check_valid_num(char *argv[]);
void	get_rules(char *argv[], t_program *prog);

// -------- Initialization ---------
int		program_initializer(char **argv, t_program *prog, t_dongle **dongles);

// ----------- Clean up ------------
void	clean_values(pthread_t monitor_thread, t_program *prog, t_dongle *dongles);
void	destroy_all(t_program *prog, t_dongle *dongles);
void	clean_failed_cond(t_dongle **dongles, int i);
void	clean_failed_mutex(t_dongle **dongles, int total_dongles, int i);

// ------------ Monitor ------------
void	*monitor(void *pointer);

// --------- Coder Routine ----------
void	*coder_routine(void *arg);

// ------------- Dongles -------------
void	lock_dongles(t_coder *coder);
void	unlock_dongles(t_coder *coder);

// ------------- Setters -------------
void	set_state(t_coder *coder, t_coder_state state);
void	set_coder_dongles(t_program *prog, t_dongle *dongles);
void	set_coder_rules(char *argv[], t_program *prog);

// ------------- Getter -------------
int				get_total_compiles(t_coder *coder);
size_t			get_program_time(size_t start);
size_t 			get_current_time(void);
t_coder_state	get_state(t_coder *coder);

// ------------ Display ------------
void	display_status(t_coder *coder);
void	display_dongle(t_coder *coder, char *dongle);

// ------------- Utils -------------
int		ft_atoi(const char *nptr);
int		ft_strcmp(const char *s1, const char *s2);
int		interruptible_sleep(t_coder *coder, int miliseconds);
char	*ft_itoa(int num);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
bool	is_cooldown(size_t last_release, size_t cooldown);
size_t	ft_strlen(const char *s);

// --------- Print Errors ----------
void	invalid_num_of_args(void);
void	invalid_num(int error_id, int index, char *value);
void	wrong_scheduler(char *value);
void	thread_errors(int error_id, int index);
void	mutex_init_errors(int error_id, int index);
void	mutex_destroy_errors(int error_id, int index);
void	cond_erors(int error_id, int index);
void	time_error(int error_id);

#endif