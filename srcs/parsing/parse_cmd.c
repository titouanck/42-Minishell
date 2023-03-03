/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:36:51 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/02 19:26:55 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_cmd(t_env *environment, char **line)
{
	t_cmd	*cmd;

	cmd = parse_args(environment, line);
	if (!(cmd))
		return (NULL);
	else if (cmd->args && !(cmd->args[0]))
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		cmd->redirect->to_execute = FALSE;
		g_returnval = 127;
		return (cmd);
	}
	else
		return (cmd);
}
