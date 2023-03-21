/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 20:01:15 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_prompt_color(t_env *environment)
{
	if (g_returnval == 0)
		environment->prompt[6] = '2';
	else
		environment->prompt[6] = '1';
}

int	main(int argc, char **argv, char *envp[])
{
	char	*line;
	t_env	*environment;

	environment = opening(argc, argv, envp);
	if (!use_argv(environment, argc, argv))
	{
		while (++(environment->line_nbr))
		{
			change_prompt_color(environment);
			line = new_prompt(environment);
			if (line && *line)
				add_history(line);
			if (!line)
				break ;
			db_free(environment->line);
			db_free(environment->last_input);
			environment->line = line;
			environment->last_input = db_strdup(line);
			parsing(environment, &line);
			rm_heredoc_files(environment);
		}
	}
	if (use_readline())
		ft_putstr_fd("exit\n", 2);
	return (closing_the_program(environment), g_returnval);
}
