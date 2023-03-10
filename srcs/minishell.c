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

static void	_export_line(t_env *environment, \
	char *line)
{
	t_env	*elem;
	
	elem = environment;
	if (!elem)
		return ;
	elem = elem->next;
	while (elem)
	{
		if (ft_strcmp(elem->key, "[minishell]_line") == 0)
		{
			if (line)
				elem->value = line;
			else
				elem->value = NULL;
			return ;
		}
		elem = elem->next;
	}
	if (line)
		env_lstaddback(environment, db_strdup("[minishell]_line"), line, 1);
}

int	main(int argc, char **argv, char *envp[]) 
{
	char	*line;
	t_env	*environment;
	int		tty;

	environment = opening(argc, argv, envp);
	if (!environment)
		return (closing_the_program(NULL), 42);
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
		dynamic_memory_address_db('+', line);
		parsing(environment, &line);
		if (line)
			db_free(line);
	}
	closing_the_program(environment);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		ft_putstr_fd("exit\n", 2);
	return (g_returnval);
}
