/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
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
		if (redirect->append = 0)
			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else
			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (redirect->fd_outfile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redirect->outfile);
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

void	last_child(t_env *environment, int pipefd[2], char **cmds, size_t size)
{
	pid_t	pid;
	t_cmd	*cmd;

	if (size > 1)
		close(pipefd[1]);
	cmd = parse_cmd(environment, cmds + size - 1);
	if (!cmd)
		return ;
	if(!_open_fds(cmd->redirect))
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		if (size > 1)
			close(pipefd[0]);
	}
	else if (pid == 0)
	{
		if (cmd->redirect->outfile)
			dup2(cmd->redirect->fd_outfile, STDOUT_FILENO);
		if (cmd->redirect->infile)
			dup2(cmd->redirect->fd_infile, STDIN_FILENO);
		else if (size > 1)
			dup2(pipefd[0], STDIN_FILENO);
		execute_cmd(environment, cmd->args);
		if (size > 1)
			close(pipefd[0]);
		if (cmd->redirect->outfile)
			close(cmd->redirect->fd_outfile);
		if (cmd->redirect->infile)
			close(cmd->redirect->fd_infile);
		free_tabstr(cmds);
		free_redirect(cmd->redirect);
		closing_the_program(environment);
		exit(g_returnval);
	}
	else
	{
		if (cmd->redirect->outfile)
			close(cmd->redirect->fd_outfile);
		if (cmd->redirect->infile)
			close(cmd->redirect->fd_infile);
		waitpid(pid, &g_returnval, 0);
		g_returnval = WEXITSTATUS(g_returnval);
		if (size > 1)
			close(pipefd[0]);
	}
	free_tabstr(cmd->args);
	free_redirect(cmd->redirect);
}
