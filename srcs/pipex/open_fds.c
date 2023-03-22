/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:53:52 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 15:57:20 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_actions_command_not_found(void)
{
	ft_putstr_fd("minishell: : command not found\n", 2);
	g_returnval = 127;
}

int	open_fds(t_env *environment, t_cmd **cmds, size_t cmdnbr)
{
	if (io_open_fds(environment, (cmds[cmdnbr])->redirect) \
	&& cmds[cmdnbr]->redirect->to_execute == TRUE)
		return (1);
	db_free(environment->log.infile);
	environment->log.infile = NULL;
	db_free(environment->log.outfile);
	environment->log.outfile = NULL;
	if (!(cmds[cmdnbr]->args) || !(cmds[cmdnbr]->args) \
	|| ((cmds[cmdnbr]->args[0]) && !((cmds[cmdnbr]->args[0])[0])))
		_actions_command_not_found();
	if ((cmds[cmdnbr])->redirect->outfile)
	{
		close((cmds[cmdnbr])->redirect->fd_outfile);
		db_free((cmds[cmdnbr])->redirect->outfile);
		(cmds[cmdnbr])->redirect->outfile = NULL;
	}
	if ((cmds[cmdnbr])->redirect->infile)
	{
		close((cmds[cmdnbr])->redirect->fd_infile);
		db_free((cmds[cmdnbr])->redirect->infile);
		(cmds[cmdnbr])->redirect->infile = NULL;
	}
	return (0);
}
