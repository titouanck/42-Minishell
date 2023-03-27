/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:02:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 16:10:26 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_returnval;

static void	init_environment(t_env *environment)
{
	environment->line = NULL;
	environment->last_input = NULL;
	environment->line_nbr = 0;
	environment->log.args = NULL;
	environment->log.infile = NULL;
	environment->log.outfile = NULL;
	environment->heredoc_files = NULL;
	environment->prompt = db_strdup("\001"GREEN">>"ENDCL \
	"\002 \001\033[34;1m\002minishell: \001\033[0m\002");
	if (!environment->prompt)
		exit_erralloc(environment);
	saved_environment(environment);
}

t_env	*opening(int argc, char **argv, char *envp[])
{
	t_env	*environment;

	init_logs();
	rl_event_hook = event;
	rl_outstream = stderr;
	(void) argc;
	(void) argv;
	g_returnval = 0;
	environment = get_environment(envp);
	if (environment)
		init_environment(environment);
	return (environment);
}
