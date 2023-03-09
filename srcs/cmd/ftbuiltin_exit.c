/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/02 19:57:29 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_i_want_to_exit(t_env *environment, t_cmd **cmds, \
unsigned char code, size_t cmdnbr)
{
	if (cmds && cmds[cmdnbr] && (cmds[cmdnbr])->redirect->infile)
	{
		close((cmds[cmdnbr])->redirect->fd_infile);
		dup2(cmds[cmdnbr]->saved_stdin, STDIN_FILENO);
		close(cmds[cmdnbr]->saved_stdin);
	}
	if (cmds && cmds[cmdnbr] && (cmds[cmdnbr])->redirect->outfile)
	{
		close((cmds[cmdnbr])->redirect->fd_outfile);
		dup2(cmds[cmdnbr]->saved_stdout, STDOUT_FILENO);
		close(cmds[cmdnbr]->saved_stdout);
	}
	free_cmds_parsed(cmds);
	closing_the_program(environment);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		ft_putstr_fd("exit\n", 2);
	exit(code);
}

void	ftbuiltin_exit(t_env *environment, char **args, t_cmd **cmds, size_t cmdnbr)
{
	unsigned char	code;
	char			*errmsg;
	size_t			i;

	code = 0;
	if (args && *args && args[1])
	{
		i = 1;
		if (!ft_inset(args[1][0], "-+0123456789"))
		{
			errmsg = ft_strrjoin("minishell: exit: ", args[1], ": numeric argument required\n");
			if (!errmsg)
				ft_putstr_fd(ERRALLOC, 2);
			else
				ft_putstr_fd(errmsg, 2);
			free(errmsg);
			_i_want_to_exit(environment, cmds, 2, cmdnbr);
			return ;
		}
		else
		{
			while (args[1][i])
			{
				if (!ft_inset(args[1][i++], "0123456789"))
				{
					errmsg = ft_strrjoin("minishell: exit: ", args[1], ": numeric argument required\n");
					if (!errmsg)
						ft_putstr_fd(ERRALLOC, 2);
					else
						ft_putstr_fd(errmsg, 2);
					free(errmsg);
					_i_want_to_exit(environment, cmds, 2, cmdnbr);
					return ;
				}
			}
		}
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_returnval = 1;
			return ;
		}
		code = (unsigned char) ft_atoi(args[1]);
	}
	_i_want_to_exit(environment, cmds, code, cmdnbr);	
}
