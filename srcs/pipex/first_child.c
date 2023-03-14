/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/14 18:14:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	first_child(t_env *environment, int pipefd[2], t_cmd **cmds)
{
	size_t	cmdnbr;
	int		status;

	cmdnbr = 0;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 0);
	if(!io_open_fds(environment, (cmds[cmdnbr])->redirect) || cmds[cmdnbr]->redirect->to_execute == FALSE)
	{
		db_free(environment->log.infile);
		environment->log.infile = NULL;
		db_free(environment->log.outfile);
		environment->log.outfile = NULL;
		if (!(cmds[cmdnbr]->args) || !(cmds[cmdnbr]->args) || ((cmds[cmdnbr]->args[0]) && !((cmds[cmdnbr]->args[0])[0])))
		{
			ft_putstr_fd("minishell: : command not found\n", 2);
			g_returnval = 127;
		}
		close(pipefd[1]);
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
		return (1);
	}
	if (ft_strcmp((cmds[cmdnbr]->args)[0], "exit") == 0)
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
	cmds[0]->pid = fork();
	if (cmds[0]->pid == -1)
	{
		db_free(environment->log.infile);
		environment->log.infile = NULL;
		db_free(environment->log.outfile);
		environment->log.outfile = NULL;
		return (perror("minishell: fork"), close(pipefd[1]), 0);
	}
	else if (cmds[0]->pid == 0)
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
		environment->log.args = (cmds[cmdnbr])->args;
		(cmds[cmdnbr])->args = NULL;
		ft_free_cmds_parsed(environment, cmds);
		closing_the_program(environment);
		exit(g_returnval);
	}
	else
	{
		db_free(environment->log.infile);
		environment->log.infile = NULL;
		db_free(environment->log.outfile);
		environment->log.outfile = NULL;
		close(pipefd[1]);
		if ((cmds[cmdnbr])->redirect->outfile)
			close((cmds[cmdnbr])->redirect->fd_outfile);
		if ((cmds[cmdnbr])->redirect->infile)
			close((cmds[cmdnbr])->redirect->fd_infile);
	}
	return (1);
}
