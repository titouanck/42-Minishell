/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 16:50:59 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_actions_child(t_env *environment, t_cmd **cmds, size_t cmdnbr, \
	int pipefd[2])
{
	close(pipefd[0]);
	if ((cmds[cmdnbr])->redirect->outfile)
	{
		close(pipefd[1]);
		dup2((cmds[cmdnbr])->redirect->fd_outfile, STDOUT_FILENO);
	}
	else
		dup2(pipefd[1], STDOUT_FILENO);
	if ((cmds[cmdnbr])->redirect->infile)
		dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
	if (!parse_builtin(environment, (cmds[cmdnbr])->args, cmds, cmdnbr))
		execute_cmd(environment, (cmds[cmdnbr])->args);
	if ((cmds[cmdnbr])->redirect->outfile == NULL)
		close(pipefd[1]);
	exit_child(environment, cmds, cmdnbr);
}

static void	_actions_parent(t_env *environment, t_cmd **cmds, size_t cmdnbr, \
	int pipefd[2])
{
	free_log_files(environment);
	close(pipefd[1]);
	if ((cmds[cmdnbr])->redirect->outfile)
		close((cmds[cmdnbr])->redirect->fd_outfile);
	if ((cmds[cmdnbr])->redirect->infile)
		close((cmds[cmdnbr])->redirect->fd_infile);
}

int	first_child(t_env *environment, int pipefd[2], t_cmd **cmds)
{
	size_t	cmdnbr;

	cmdnbr = 0;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 0);
	if (!open_fds(environment, cmds, cmdnbr))
	{
		close(pipefd[1]);
		return (1);
	}
	cmds[0]->pid = fork();
	if (cmds[0]->pid == -1)
	{
		free_log_files(environment);
		return (perror("minishell: fork"), close(pipefd[1]), 0);
	}
	else if (cmds[0]->pid == 0)
		_actions_child(environment, cmds, cmdnbr, pipefd);
	else
		_actions_parent(environment, cmds, cmdnbr, pipefd);
	return (1);
}
