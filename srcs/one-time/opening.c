/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:02:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 18:44:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_returnval;

int	event(void)
{
	return (42);
}

t_env *opening(int argc, char **argv, char *envp[])
{
	t_env	*environment;
	char	**cmd;
	pid_t	pid;

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
		exit(4);
	}
	else
		wait(NULL);
	pid = fork();
	if (pid == 0)
	{
		cmd = ft_split("/usr/bin/rm rm -rf .logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		cmd = ft_split("/bin/rm rm -rf .logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		exit(8);
	}
	else
		wait(NULL);
	pid = fork();
	if (pid == 0)
	{
		cmd = ft_split("/usr/bin/mkdir mkdir -p .logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		cmd = ft_split("/bin/mkdir mkdir -p .logs", ' ');
		if (cmd)
		{
			execve(cmd[0], cmd + 1, NULL);
			ft_freetab(cmd);
		}
		exit(15);
	}
	else
		wait(NULL);
	rl_event_hook = event;
	rl_outstream = stderr;
	(void) argc;
	(void) argv;
	g_returnval = 0;
	environment = get_environment(envp);
	if (environment)
		environment->args = NULL;
	return (environment);
}
