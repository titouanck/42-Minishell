/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:03:48 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 16:19:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_args(t_env *environment, char **line)
{
	char	**args;

	(void) environment;
	if (!quotes_interpretation(environment, line))
		return (NULL);
	args = ft_split(*line, SEPARATOR);
	if (!args)
	{
		ft_putstr_fd(ERRALLOC, 2);
		g_returnval = 12;
		free(*line);
		closing_the_program(environment);
		exit(g_returnval);
	}
	return (args);
}
