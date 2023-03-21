/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:02:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 19:57:55 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_returnval;

static void	init_environment(t_env *environment)
{
	char	*tab[3];

	environment->line = NULL;
	environment->last_input = NULL;
	environment->line_nbr = 0;
	environment->log.args = NULL;
	environment->log.infile = NULL;
	environment->log.outfile = NULL;
	environment->heredoc_files = NULL;
	environment->prompt = db_strdup("\r\r\001"GREEN"●"ENDCL"\002 \001\033[34;1m\002minishell \001→\002 \001\033[0m\002");
	if (!environment->prompt)
		exit_erralloc(environment);
	tab[0] = "export";
	tab[1] = "_=minishell/env";
	tab[2] = NULL;
	ftbuiltin_export(environment, tab);
}

t_env *opening(int argc, char **argv, char *envp[])
{
	t_env	*environment;

	init_logs();
	rl_event_hook = event;
	rl_outstream = stderr;
	(void) argc;
	(void) argv;
	g_returnval = 0;
	environment = get_environment(envp);
	init_environment(environment);
	return (environment);
}
