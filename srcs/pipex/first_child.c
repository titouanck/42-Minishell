/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 13:02:20 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	first_child(t_env *environment, int pipefd[2], t_cmd **cmds)
{
	pid_t	pid;
	size_t	cmdnbr;

	cmdnbr = 0;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 0);
	if(cmds[cmdnbr]->redirect->to_execute == FALSE || !io_open_fds((cmds[cmdnbr])->redirect))
	{
		close(pipefd[1]);
		return (1);
	}
	if (ft_strcmp((cmds[cmdnbr]->args)[0], "exit") == 0)
	// if (ft_strcmp((cmds[cmdnbr]->args)[0], "exit") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "cd") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "unset") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "export") == 0)
	{
		if ((cmds[cmdnbr])->redirect->infile)
		{
			cmds[cmdnbr]->saved_stdin = dup(0);
			dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
		}
		if ((cmds[cmdnbr])->redirect->outfile)
		{
			close(pipefd[1]);
			cmds[cmdnbr]->saved_stdout = dup(1);
			dup2((cmds[cmdnbr])->redirect->fd_outfile, STDOUT_FILENO);
		}
		parse_builtin(environment, (cmds[cmdnbr])->args, cmds, cmdnbr);
		if ((cmds[cmdnbr])->redirect->infile)
		{
			close((cmds[cmdnbr])->redirect->fd_infile);
			dup2(cmds[cmdnbr]->saved_stdin, STDIN_FILENO);
			close(cmds[cmdnbr]->saved_stdin);
		}
		if ((cmds[cmdnbr])->redirect->outfile)
		{
			close((cmds[cmdnbr])->redirect->fd_outfile);
			dup2(cmds[cmdnbr]->saved_stdout, STDOUT_FILENO);
			close(cmds[cmdnbr]->saved_stdout);
		}
		else
			close(pipefd[1]);
		return (1);
	}
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), close(pipefd[1]), 0);
	else if (pid == 0)
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
		if ((cmds[cmdnbr])->redirect->outfile)
			close((cmds[cmdnbr])->redirect->fd_outfile);
		if ((cmds[cmdnbr])->redirect->infile)
			close((cmds[cmdnbr])->redirect->fd_infile);
		free_cmds_parsed(cmds);
		closing_the_program(environment);
		exit(g_returnval);
	}
	else
	{
		close(pipefd[1]);
		if ((cmds[cmdnbr])->redirect->outfile)
			close((cmds[cmdnbr])->redirect->fd_outfile);
		if ((cmds[cmdnbr])->redirect->infile)
			close((cmds[cmdnbr])->redirect->fd_infile);
	}
	return (1);
}
