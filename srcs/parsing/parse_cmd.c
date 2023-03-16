/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:36:51 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/16 15:01:56 by tchevrie         ###   ########.fr       */
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
		cmd->redirect->to_execute = FALSE;
		return (cmd);
	}
	else
		return (cmd);
}
