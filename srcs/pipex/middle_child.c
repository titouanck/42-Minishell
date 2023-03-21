/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 18:53:15 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	middle_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr)
{
	int		new_pipefd[2];

	if (pipe(new_pipefd) == -1)
		return (perror("minishell: pipe"), close(pipefd[0]), 0);
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
		close(pipefd[0]);
		close(new_pipefd[1]);
		pipefd[0] = new_pipefd[0];
		pipefd[1] = new_pipefd[1];
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
	cmds[cmdnbr]->pid = fork();
	if (cmds[cmdnbr]->pid == -1)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(new_pipefd[0]);
		close(new_pipefd[1]);
		db_free(environment->log.infile);
		environment->log.infile = NULL;
		db_free(environment->log.outfile);
		environment->log.outfile = NULL;
		return (0);
	}
	else if (cmds[cmdnbr]->pid == 0)
	{
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
	else
	{
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
	return (1);
}
