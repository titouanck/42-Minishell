/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 16:53:32 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_actions_child(t_cmd **cmds, size_t cmdnbr, \
	int pipefd[2], int new_pipefd[2])
{
	t_env	*environment;

	environment = saved_environment(NULL);
	close(new_pipefd[0]);
	if ((cmds[cmdnbr])->redirect->outfile)
	{
		close(new_pipefd[1]);
		dup2((cmds[cmdnbr])->redirect->fd_outfile, STDOUT_FILENO);
	}
	else
		dup2(new_pipefd[1], STDOUT_FILENO);
	if ((cmds[cmdnbr])->redirect->infile)
	{
		close(pipefd[0]);
		dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
	}
	else
		dup2(pipefd[0], STDIN_FILENO);
	if (!parse_builtin(environment, (cmds[cmdnbr])->args, cmds, cmdnbr))
		execute_cmd(environment, (cmds[cmdnbr])->args);
	if ((cmds[cmdnbr])->redirect->outfile == NULL)
		close(new_pipefd[1]);
	if ((cmds[cmdnbr])->redirect->infile == NULL)
		close(pipefd[0]);
	exit_child(environment, cmds, cmdnbr);
}

static void	_actions_parent(t_cmd **cmds, size_t cmdnbr, \
	int pipefd[2], int new_pipefd[2])
{
	t_env	*environment;

	environment = saved_environment(NULL);
	db_free(environment->log.infile);
	environment->log.infile = NULL;
	db_free(environment->log.outfile);
	environment->log.outfile = NULL;
	close(pipefd[0]);
	close(new_pipefd[1]);
	pipefd[0] = new_pipefd[0];
	pipefd[1] = new_pipefd[1];
	if ((cmds[cmdnbr])->redirect->outfile)
		close((cmds[cmdnbr])->redirect->fd_outfile);
	if ((cmds[cmdnbr])->redirect->infile)
		close((cmds[cmdnbr])->redirect->fd_infile);
}

int	middle_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr)
{
	int		new_pipefd[2];

	if (pipe(new_pipefd) == -1)
		return (perror("minishell: pipe"), close(pipefd[0]), 0);
	if (!open_fds(environment, cmds, cmdnbr))
	{
		close(pipefd[0]);
		close(new_pipefd[1]);
		pipefd[0] = new_pipefd[0];
		pipefd[1] = new_pipefd[1];
		return (1);
	}
	cmds[cmdnbr]->pid = fork();
	if (cmds[cmdnbr]->pid == -1)
	{
		close(pipefd[0]);
		close(new_pipefd[0]);
		close(new_pipefd[1]);
		free_log_files(environment);
		perror("minishell: fork");
		return (0);
	}
	else if (cmds[cmdnbr]->pid == 0)
		return (_actions_child(cmds, cmdnbr, pipefd, new_pipefd), 0);
	return (_actions_parent(cmds, cmdnbr, pipefd, new_pipefd), 1);
}
