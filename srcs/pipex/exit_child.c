/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:49:41 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 16:49:56 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_child(t_env *environment, t_cmd **cmds, size_t cmdnbr)
{
	if ((cmds[cmdnbr])->redirect->outfile)
		close((cmds[cmdnbr])->redirect->fd_outfile);
	if ((cmds[cmdnbr])->redirect->infile)
		close((cmds[cmdnbr])->redirect->fd_infile);
	environment->log.args = (cmds[cmdnbr])->args;
	(cmds[cmdnbr])->args = NULL;
	ft_free_cmds_parsed(cmds);
	closing_the_program(environment);
	exit(g_returnval);
}
