/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/13 11:55:13 by tchevrie         ###   ########.fr       */
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
	while (++(environment->line_nbr))
	{
		tty = -1;
		if (use_readline())
		{
			if (tty != 1)
			{
				default_signal_behavior();
				tty = 1;
			}
			line = readline("\033[34;1m" PROMPT ENDCL " ");
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
