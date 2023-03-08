/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 19:05:23 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	middle_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr)
{
	pid_t	pid;
	int		new_pipefd[2];

	if (pipe(new_pipefd) == -1)
		return (perror("minishell: pipe"), close(pipefd[0]), 0);
	if(cmds[cmdnbr]->redirect->to_execute == FALSE || !io_open_fds((cmds[cmdnbr])->redirect))
	{
		if ((!(cmds[cmdnbr]->args) || (!(cmds[cmdnbr]->args)[0])) && cmds[cmdnbr]->empty_cmd == 1)
		{
			ft_putstr_fd("minishell: : command not found\n", 2);
			g_returnval = 127;
		}
		close(pipefd[0]);
		close(new_pipefd[1]);
		pipefd[0] = new_pipefd[0];
		pipefd[1] = new_pipefd[1];
		return (1);
	}
	if (ft_strcmp((cmds[cmdnbr]->args)[0], "exit") == 0)
	// if (ft_strcmp((cmds[cmdnbr]->args)[0], "exit") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "cd") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "unset") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "export") == 0)
	{
		if ((cmds[cmdnbr])->redirect->infile)
		{
			close(pipefd[0]);
			cmds[cmdnbr]->saved_stdin = dup(0);
			dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
		}
		if ((cmds[cmdnbr])->redirect->outfile)
		{
			close(new_pipefd[1]);
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
		else
			close(pipefd[0]);
		if ((cmds[cmdnbr])->redirect->outfile)
		{
			close((cmds[cmdnbr])->redirect->fd_outfile);
			dup2(cmds[cmdnbr]->saved_stdout, STDOUT_FILENO);
			close(cmds[cmdnbr]->saved_stdout);
		}
		else
			close(new_pipefd[1]);
		close(pipefd[0]);
		close(new_pipefd[1]);
		pipefd[0] = new_pipefd[0];
		pipefd[1] = new_pipefd[1];
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(new_pipefd[0]);
		close(new_pipefd[1]);
		return (0);
	}
	else if (pid == 0)
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
		free_cmds_parsed(cmds);
		closing_the_program(environment);
		exit(g_returnval);
	}
	else
	{
		close(pipefd[0]);
		close(new_pipefd[1]);
		pipefd[0] = new_pipefd[0];
		pipefd[1] = new_pipefd[1];
	}
	return (1);
}
