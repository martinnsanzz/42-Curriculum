/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 2002mssm02 <2002mssm02@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:15:51 by masanz-s          #+#    #+#             */
/*   Updated: 2026/10/05 13:08:44 by 2002mssm02       ###   ########.fr       */
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
# define LOG_TAKE_DONGLE	"%s[%lu] Coder %d has taken %s dongle%s\n"
# define LOG_COMPILING		"%s[%lu] Coder %d is compiling%s\n"
# define LOG_DEBUGGING		"%s[%lu] Coder %d is debugging%s\n"
# define LOG_REFACTOR		"%s[%lu] Coder %d is refactoring%s\n"
# define LOG_BURNS_OUT		"%s[%lu] Coder %d burned out%s\n"
# define LOG_SUCCESS		"\n%sAll coders have compiled.%s\n"

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

typedef enum e_dongle_state
{
	DONGLE_FREE,
	DONGLE_TAKEN
}	t_dongle_state;

// -------------------- //
//		STRUCTURES		//
// -------------------- //
typedef struct s_program	t_program;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef struct s_schedule	t_schedule;
typedef struct s_request	t_request;

typedef struct s_program
{
	pthread_t		monitor_thread;
	int				compiles_required;
	int				total_coders;

	size_t			start_time;

	bool			burn_out_flag;
	bool			all_finish;

	t_coder			*coders;

	t_schedule		*scheduler;

	pthread_mutex_t	compile_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	state_lock;
}	t_program;

typedef struct s_schedule
{
	pthread_t		scheduler_thread;

	char			*sched_arg;

	bool			*burn_out;
	bool			*all_finish;

	t_request		*heap;
	size_t			heap_size;
	size_t			next_seq;

	pthread_mutex_t	priority_lock;
	pthread_cond_t	turn_cond;
}	t_schedule;

typedef struct s_dongle
{
	t_dongle_state	state;
	size_t			last_release;

	pthread_mutex_t	lock;
}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread;

	int				id;
	int				total_compiles;
	int				*compiles_required;
	int				*total_coders;

	bool			priority;
	bool			*burn_out;

	size_t			time_to_burn_out;
	size_t			time_to_compile;
	size_t			time_to_debug;
	size_t			time_to_refactor;
	size_t			last_compile;
	size_t			dongle_cooldown;
	size_t			*start_time;

	t_coder_state	state;

	t_dongle		*l_dongle;
	t_dongle		*r_dongle;

	t_schedule		*schedule;

	pthread_mutex_t	*compile_lock;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*state_lock;
}	t_coder;

typedef struct s_request
{
	t_coder	*coder;
	size_t	key;
	size_t	seq;
}	t_request;

// -------------------- //
//		PROTOTYPES		//
// -------------------- //

// ----------- Parsing -------------
int				check_argv(int argc, char *argv[]);
int				check_valid_num(char *argv[]);

// -------- Initialization ---------
// Structs //
int				init_program(t_program *prog, char *argv[]);
int				init_dongles(int total_dongles, t_dongle **dongles);
int				init_coders(t_program *prog, t_dongle *dongles, char *argv[]);
int				init_scheduler(t_program *prog, char *argv[]);

// Threads //
int				init_monitor_thread(t_program *prog);
int				init_scheduler_thread(t_schedule *scheduler);
int				init_coder_threads(t_program *prog);

// ----------- Clean up ------------
void			clean_values(t_program *prog, t_dongle *dongles);
void			destroy_all(t_program *prog, t_dongle *dongles);
void			clean_failed_mutex(t_dongle **dongles, int i);
void			clean_dongles(t_dongle **dongles, int total_dongles);
void			cleanup_pre_threads(t_program *prog, t_dongle *dongles);
void			cleanup_after_monitor(t_program *prog, t_dongle *dongles);
void			free_scheduler(t_schedule **scheduler);

// ----------- Schedule ------------
size_t			compute_key(t_schedule *s, t_coder *coder);
void			*sched_routine(void *pointer);
int				wait_for_turn(t_coder *coder);
void			wake_scheduler(t_schedule *s);

// ------------ Monitor ------------
void			*monitor(void *pointer);

// --------- Coder Routine ----------
void			*coder_routine(void *arg);

// ------------- Dongles -------------
void			take_dongles(t_coder *coder);
void			unlock_dongles(t_coder *coder);
void			reserve_dongles(t_coder *coder);
bool			can_grant(t_coder *coder, size_t *wake_at);

// ------------- Setters -------------
void			set_coder_state(t_coder *coder, t_coder_state state);
void			set_dongle_state(t_dongle *dongle, t_dongle_state state);
void			set_coder_dongles(t_coder *coder, t_dongle *dongles, int index);
void			set_coder_rules(t_coder *coder, t_program *prog, char *argv[]);

// ------------- Getter -------------
int				get_total_compiles(t_coder *coder);
size_t			get_program_time(size_t start);
size_t			get_current_time(void);
t_coder_state	get_coder_state(t_coder *coder);
t_dongle_state	get_dongle_state(t_dongle *dongle, size_t *last_release);

// ------------ Display ------------
void			display_status(t_coder *coder);
void			display_dongle(t_coder *coder, char *dongle);

// ------------- Utils -------------
int				ft_atoi(const char *nptr);
int				ft_strcmp(const char *s1, const char *s2);
int				interruptible_sleep(t_coder *coder, int miliseconds);
char			*ft_itoa(int num);
void			ft_bzero(void *s, size_t n);
void			*ft_memset(void *s, int c, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
bool			is_cooldown(size_t last_release, size_t cooldown);
size_t			ft_strlen(const char *s);
t_coder			*heap_pop(t_schedule *s);
int				heap_push(t_schedule *s, t_coder *coder);
void			heap_swap(t_request *a, t_request *b);
void			sift_down(t_schedule *s, size_t i);
void			heap_remove_at(t_schedule *s, size_t i);
int				find_grantable(t_schedule *s, size_t *wake_at);

// --------- Print Errors ----------
void			invalid_num_of_args(void);
void			invalid_num(int error_id, int index, char *value);
void			wrong_scheduler(char *value);
void			thread_errors(int error_id, int index);
void			mutex_init_errors(int error_id, int index);
void			mutex_destroy_errors(int error_id, int index);
void			cond_erors(int error_id);
void			time_error(int error_id);

#endif