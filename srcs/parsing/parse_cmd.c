/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:36:51 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/27 16:58:45 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_cmd(t_env *environment, char **line)
{
	char	**args;
	
	args = parse_args(environment, line);
	if (!args)
		return (NULL);
	else if (!args[0])
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		free_tabstr(args);
		g_returnval = 127;
		return (NULL);
	}
	else
		return (args);
}
