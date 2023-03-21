/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_logs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:06:39 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 19:20:54 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_remove_logfile(void)
{
	pid_t	pid;
	char	**cmd;
	
	pid = fork();
	if (pid == 0)
	{
		cmd = ft_split("/usr/bin/rm rm -f assets/minishell.log", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		cmd = ft_split("/bin/rm rm -f assets/minishell.log", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		closing_the_program(NULL);
		exit(127);
	}
	else
		wait(NULL);
}

static void	_remove_logfolder(void)
{
	pid_t	pid;
	char	**cmd;
	
	pid = fork();
	if (pid == 0)
	{
		cmd = ft_split("/usr/bin/rm rm -rf /tmp/.minishell-logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		cmd = ft_split("/bin/rm rm -rf /tmp/.minishell-logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		closing_the_program(NULL);
		exit(127);
	}
	else
		wait(NULL);
}

static void	_create_logfolder(void)
{
	pid_t	pid;
	char	**cmd;

	pid = fork();
	if (pid == 0)
	{
		cmd = ft_split("/usr/bin/mkdir mkdir -p /tmp/.minishell-logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		cmd = ft_split("/bin/mkdir mkdir -p /tmp/.minishell-logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		closing_the_program(NULL);
		exit(127);;
	}
	else
		wait(NULL);
}

void	init_logs(void)
{
	_remove_logfile();
	_remove_logfolder();
	_create_logfolder();
}