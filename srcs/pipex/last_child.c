/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/02 18:24:35 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	_open_fds(t_redirect *redirect)
{
	if (!redirect)
		return (0);
	if (redirect->infile)
	{
		redirect->fd_infile = open(redirect->infile, O_RDONLY);
		if (redirect->fd_infile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redirect->infile);
			free(redirect->infile);
			redirect->infile = NULL;
			free(redirect->outfile);
			redirect->outfile = NULL;
			return (0);
		}
	}
	if (redirect->outfile)
	{
		if (redirect->append == 0)
			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else
			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (redirect->fd_outfile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redirect->outfile);
			if (redirect->infile)
				close(redirect->fd_infile);
			free(redirect->infile);
			redirect->infile = NULL;
			free(redirect->outfile);
			redirect->outfile = NULL;
			return (0);
		}
	}
	return (1);
}

void	last_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr)
{
	pid_t	pid;

	if(cmds[cmdnbr]->redirect->to_execute == FALSE || !_open_fds((cmds[cmdnbr])->redirect))
	{
		if (cmdnbr != 0)
			close(pipefd[0]);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		if (cmdnbr != 0)
			close(pipefd[0]);
	}
	else if (pid == 0)
	{
		if ((cmds[cmdnbr])->redirect->outfile)
			dup2((cmds[cmdnbr])->redirect->fd_outfile, STDOUT_FILENO);
		if ((cmds[cmdnbr])->redirect->infile)
			dup2((cmds[cmdnbr])->redirect->fd_infile, STDIN_FILENO);
		else if (cmdnbr != 0)
			dup2(pipefd[0], STDIN_FILENO);
		execute_cmd(environment, (cmds[cmdnbr])->args);
		if (cmdnbr != 0)
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
		if ((cmds[cmdnbr])->redirect->outfile)
			close((cmds[cmdnbr])->redirect->fd_outfile);
		if ((cmds[cmdnbr])->redirect->infile)
			close((cmds[cmdnbr])->redirect->fd_infile);
		waitpid(pid, &g_returnval, 0);
		g_returnval = WEXITSTATUS(g_returnval);
		if (cmdnbr != 0)
			close(pipefd[0]);
	}
}
