/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:02:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 16:53:56 by tchevrie         ###   ########.fr       */
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
	(void) argc;
	(void) argv;
	g_returnval = 0;
	default_signal_behavior();
	environment = get_environment(envp);
	return (environment);
}
