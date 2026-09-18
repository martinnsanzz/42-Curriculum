/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masanz-s <masanz-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:47:15 by masanz-s          #+#    #+#             */
/*   Updated: 2026/09/18 15:23:28 by masanz-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void display_status(int start_time, int coder_id, t_coder_state state)
{
	int		prog_time;

	prog_time = get_program_time(start_time, get_current_time());
    if (state == TAKING_DONGLE)
        printf("\033[30m%d %d has taken dongle\n", prog_time, coder_id);
    else if (state == COMPILING)
        printf("\033[32m%d %d is compiling\n", prog_time, coder_id);
    else if (state == DEBUGGING)
        printf("\033[35m%d %d is debugging\n", prog_time, coder_id);
    else if (state == REFACTORING)
        printf("\033[37m%d %d is refactoring\n", prog_time, coder_id);
    else if (state == BURNOUT)
        printf("\033[0;31m%d %d burned out\n", prog_time, coder_id);
    printf("\033[0m");
}

