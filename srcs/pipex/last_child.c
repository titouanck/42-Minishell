/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 16:07:41 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_for_sigint(t_cmd **cmds)
{
	int		status;
	size_t	i;

	if (!cmds || !(*cmds))
		return (0);
	status = 0;
	i = 0;
	while (cmds[i])
	{
		waitpid(cmds[i]->pid, &status, 0);
		if (status == 2)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_done = 1;
			if (!use_readline())
				return (status);
		}
		i++;
	}
	return (status);
}

void	last_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr)
{
	if (!open_fds(environment, cmds, cmdnbr))
	{
		if (cmdnbr != 0)
			close(pipefd[0]);
		return ;
	}
	if (cmdnbr == 0 && (ft_strcmp((cmds[cmdnbr]->args)[0], "cd") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "exit") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "export") == 0 || ft_strcmp((cmds[cmdnbr]->args)[0], "unset") == 0))
	{
		if (cmdnbr != 0)
			close(pipefd[0]);
		if ((cmds[cmdnbr])->redirect->infile)
		{
			cmds[cmdnbr]->saved_stdin = dup(0);
			dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
		}
		if ((cmds[cmdnbr])->redirect->outfile)
		{
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
		return ;
	}
	cmds[cmdnbr]->pid = fork();
	if (cmds[cmdnbr]->pid == -1)
	{
		free_log_files(environment);
		perror("minishell: fork");
		if (cmdnbr != 0)
			close(pipefd[0]);
	}
	else if (cmds[cmdnbr]->pid == 0)
	{
		if ((cmds[cmdnbr])->redirect->outfile)
			dup2((cmds[cmdnbr])->redirect->fd_outfile, STDOUT_FILENO);
		if ((cmds[cmdnbr])->redirect->infile)
		{
			dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
			if (cmdnbr != 0)
				close(pipefd[0]);
		}
		else if (cmdnbr != 0)
			dup2(pipefd[0], STDIN_FILENO);
		if (!parse_builtin(environment, (cmds[cmdnbr])->args, cmds, cmdnbr))
			execute_cmd(environment, (cmds[cmdnbr])->args);
		if (cmdnbr != 0 && (cmds[cmdnbr])->redirect->infile == NULL)
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
		if (cmdnbr != 0)
			close(pipefd[0]);
		free_log_files(environment);
		if ((cmds[cmdnbr])->redirect->outfile)
			close((cmds[cmdnbr])->redirect->fd_outfile);
		if ((cmds[cmdnbr])->redirect->infile)
			close((cmds[cmdnbr])->redirect->fd_infile);
		g_returnval = check_for_sigint(cmds);
		if (g_returnval == 2)
		{
			g_returnval = 130;
			if (!use_readline())
			{
				if (cmdnbr != 0)
					close(pipefd[0]);
				closing_the_program(environment);
				exit(g_returnval);
			}
		}
		else if (g_returnval > 255)
			g_returnval = WEXITSTATUS(g_returnval);
	}
}
