/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/14 09:52:16 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void display_status(int timestamp_ms, int coder_id, char *state)
{
    char *timestamp_str;
    char *id_str;

    timestamp_str = ft_itoa(timestamp_ms);
    id_str = ft_itoa(coder_id);

    if (ft_strcmp(state, "dongle") == 0)
        printf("\033[30m%s %s has taken dongle\n", timestamp_str, id_str);
    else if (ft_strcmp(state, "compiling") == 0)
        printf("\033[32m%s %s is compiling\n", timestamp_str, id_str);
    else if (ft_strcmp(state, "debugging") == 0)
        printf("\033[35m%s %s is debugging\n", timestamp_str, id_str);
    else if (ft_strcmp(state, "refactoring") == 0)
        printf("\033[37m%s %s is refactoring\n", timestamp_str, id_str);
    else if (ft_strcmp(state, "burn out") == 0)
        printf("\033[0;31m%s %s burned out\n", timestamp_str, id_str);
    printf("\033[0m");
	usleep(timestamp_ms);
}
