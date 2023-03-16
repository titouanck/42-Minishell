/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:02:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/16 16:10:06 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_returnval;

int	event(void)
{
	return (42);
}

void    ft_logo(void)
{
    const char *logo;
	
	logo = "\t\t\t\t\t\t\t\t\t\t      |\
	\n|   /██      /██ /██████ /██   /██ /██████  /██████  /██   /██ /████████ /██       /██ \t      |\
	\n|  | ███    /███|_  ██_/| ███ | ██|_  ██_/ /██__  ██| ██  | ██| ██_____/| ██      | ██ \t      |\
	\n|  | ████  /████  | ██  | ████| ██  | ██  | ██  \\__/| ██  | ██| ██      | ██      | ██ \t      |\
	\n|  | ██ ██/██ ██  | ██  | ██ ██ ██  | ██  |  ██████ | ████████| █████   | ██      | ██ \t      |\
	\n|  | ██  ███| ██  | ██  | ██  ████  | ██   \\____  ██| ██__  ██| ██__/   | ██      | ██ \t      |\
	\n|  | ██\\  █ | ██  | ██  | ██\\  ███  | ██   /██  \\ ██| ██  | ██| ██      | ██      | ██ \t      |\
	\n|  | ██ \\/  | ██ /██████| ██ \\  ██ /██████|  ██████/| ██  | ██| ████████| ████████| ████████  |\
	\n|  |__/     |__/|______/|__/  \\__/|______/ \\______/ |__/  |__/|________/|________/|________/  | ";
	ft_printf("\e[32;1m ---------------------------------------------------------------------------------------------\n|\
	%s\n|\t\t\t\t\t\t\t\t\t\t\t      |\n ---------------------------------------------------------------------------------------------\n\n\e[0m", logo);
    
}

t_env *opening(int argc, char **argv, char *envp[])
{
	t_env	*environment;
	char	**cmd;
	pid_t	pid;

	ft_logo();
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
		exit(8);
	}
	else
		wait(NULL);
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
	return (environment);
}
