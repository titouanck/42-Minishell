/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/14 17:46:15 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	use_readline(void)
{
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		return (TRUE);
	else
		return (FALSE);
}

int	main(int argc, char **argv, char *envp[]) 
{
	char	*line;
	t_env	*environment;
	int		tty;

	environment = opening(argc, argv, envp);
	if (!environment)
		return (closing_the_program(NULL), 42);
	environment->line = NULL;
	environment->last_input = NULL;
	environment->line_nbr = 0;
	environment->log.args = NULL;
	environment->log.infile = NULL;
	environment->log.outfile = NULL;
	environment->prompt = db_strdup("\r\r\001"GREEN"●"ENDCL"\002 \001\033[34;1m\002minishell \001→\002 \001\033[0m\002");
	if (!environment->prompt)
	{
		g_returnval = 1;
		closing_the_program(environment);
		ft_putstr_fd(ERRALLOC, 2);
		return (g_returnval);
	}
	while (++(environment->line_nbr))
	{
		if (g_returnval == 0)
			environment->prompt[6] = '2';
		else
			environment->prompt[6] = '1';
		tty = -1;
		if (use_readline())
		{
			if (tty != 1)
			{
				default_signal_behavior();
				tty = 1;
			}
			line = readline(environment->prompt);
			dynamic_memory_address_db('+', line);
		}
		else
		{
			if (tty != 0)
			{
				notatty_signal_behavior();
				tty = 0;
			}
			line = get_next_line(0);
			if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		if (line && *line)
			add_history(line);
		if (!line)
			break ;
		db_free(environment->line);
		db_free(environment->last_input);
		environment->line = line;
		environment->last_input = db_strdup(line);
		parsing(environment, &line);
	}
	closing_the_program(environment);
	if (use_readline())
		ft_putstr_fd("exit\n", 2);
	return (g_returnval);
}
