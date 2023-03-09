/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 13:50:09 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char *envp[]) 
{
	char	*line;
	t_env	*environment;

	environment = opening(argc, argv, envp);
	if (!environment)
		return (42);
	while (1)
	{
		line = readline("\033[34;1m" PROMPT ENDCL " ");
		// printf("\033[37;43m%s"ENDCL"\n\n", line);
		if (line && *line)
			add_history(line);
		if (!line)
			break ;
		parsing(environment, &line);
		if (line)
			free(line);
	}
	closing_the_program(environment);
	ft_putstr_fd("exit\n", 2);
	return (g_returnval);
}
