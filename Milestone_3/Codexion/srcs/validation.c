/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 12:16:06 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 11:46:23 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_isnumber(char *s);

int     check_argv(int argc, char *argv[])
{
    fprintf(stderr, "\033[0;31m");
	if (argc != 9){
		fprintf(stderr, "Program must have 8 arguments !!\n"
						" Run 'make help-run' for help.\n");
		return (1);
	}
	else if ((ft_strcmp(argv[8], "fifo")) && (ft_strcmp(argv[8], "edf"))){
		fprintf(stderr, "Wrong scheduler {%s}: Allowed ['fifo', 'edf']\n", argv[8]);
		return (1);
	}
	if (check_valid_num(argv) == 1)
		return (1);
	fprintf(stderr, "\033[0m");
	return (0);
}

int check_valid_num(char *argv[])
{
	int i;

    i = 0;
	while (i++ < 7){
		if (ft_isnumber(argv[i]) == 0){
			fprintf(stderr, "Argument %d is an invalid int: {%s}\n", i, argv[i]);
			return (1);
		}
		else if (ft_atoi(argv[i]) == 0 && ft_strlen(argv[i]) > 1){
			fprintf(stderr, "Value '%d' can't be a greater than INT_MAX: {%s}\n", i, argv[i]);
            return(1);
		}
		else if (ft_atoi(argv[i]) == 0 && ft_strlen(argv[i]) == 1){
			fprintf(stderr, "Value '%d' must be greater than 0: {%s}\n", i, argv[i]);
            return(1);
		}
		else if (ft_atoi(argv[i]) < 0){
            fprintf(stderr, "Value '%d' can't be a negative value: {%s}\n", i, argv[i]);
            return(1);
        }
	}
	return (0);
}

void    get_rules(char *argv[], int *arr, char **scheduler)
{
    int i;

    i = 0;
    while(i++ < 7)
		arr[i - 1] = (int)ft_atoi(argv[i]);
	*scheduler = argv[8];
}

static int	ft_isnumber(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		if (!(s[i] >= '0' && s[i] <= '9') && s[i] != '-')
				return (0);
		i++;
	}
	return (1);
}
