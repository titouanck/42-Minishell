/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_erralloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 17:18:59 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 17:19:08 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_erralloc(t_env *environment)
{
	g_returnval = 1;
	closing_the_program(environment);
	ft_putstr_fd(ERRALLOC, 2);
	exit(g_returnval);
}
