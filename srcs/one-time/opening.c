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

	rl_event_hook = event;
	rl_outstream = stderr;
	(void) argc;
	(void) argv;
	g_returnval = 0;
	environment = get_environment(envp);
	if (!environment)
		return (NULL);
	environment->fd_log = -1;
	if (access("assets/", X_OK) == 0)
	{
		environment->fd_log = open("./assets/minishell.log", O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (environment->fd_log == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror("open");
			g_returnval = 1;
		}
		else
			ft_putstr_fd("minishell: open: Success.\n\n", environment->fd_log);
	}
	return (environment);
}
