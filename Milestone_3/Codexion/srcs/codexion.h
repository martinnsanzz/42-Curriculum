/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:15:51 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/16 14:29:44 by masanz-s         ###   ########.fr       */
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

typedef enum e_dongle_state
{
	AVAILABLE,
	UNAVAILABLE
}	t_dongle_state;

// ---------- Structs -----------
typedef struct s_coder
{
	int				coder_id;

	int				left_dongle_i;
	int				right_dongle_i;

	int				total_compiles;

	t_coder_state	coder_state;
} t_coder;

typedef struct s_dongle
{
	int				dongle_id;

	pthread_mutex_t	lock;

	t_dongle_state	dongle_state;
} t_dongle;

typedef struct s_program
{
    t_coder			*coders;
    t_dongle		*dongles;
    pthread_t		*threads;
    t_thread_arg    *args;
    char            *scheduler;
    int				rules[7];
}	t_program;

typedef struct s_thread_arg
{
	t_coder			*coder;
	t_program		*prog;
}	t_thread_arg;



// -------------- Test -------------
void		*print_hello(void *arg);

// -------- Initialization ---------
int         data_initializer(t_program *prog);
int			init_threads(t_program *prog);

// ----------- Clean up ------------
void		clean_values(t_program *prog);

// ---------- Validation -----------
int			check_argv(int argc, char *argv[]);
int			check_valid_num(char *argv[]);
void		get_rules(char *argv[], t_program *prog);

// ------------ Display ------------
void		display_status(int timestamp_ms, int coder_id, char *state);

// ------------- Utils -------------
long long	ft_atoi(const char *nptr);
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