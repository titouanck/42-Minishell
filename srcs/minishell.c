/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 18:43:34 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char *envp[]) 
{
	char	*line;
	t_env	*environment;
	int		tty;

	environment = opening(argc, argv, envp);
	if (!environment)
		return (42);
	while (1)
	{
		tty = -1;
		if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		{
			if (tty != 1)
			{
				default_signal_behavior();
				tty = 1;
			}
			line = readline("\033[34;1m" PROMPT ENDCL " ");
		}
		else
		{
			if (tty != 0)
			{
				notatty_signal_behavior();
				tty = 0;
			}
			line = get_next_line(0);
		}
		if (line && *line)
			add_history(line);
		if (!line)
			break ;
		parsing(environment, &line);
		if (line)
			free(line);
	}
	closing_the_program(environment);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		ft_putstr_fd("exit\n", 2);
	return (g_returnval);
}
