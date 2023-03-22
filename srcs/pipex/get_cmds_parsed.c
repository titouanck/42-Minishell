/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds_parsed.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:11:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 15:14:34 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*_get_cmd_parsed(char **cmds, t_cmd **cmds_parsed, \
	size_t i, size_t size)
{
	t_env	*environment;

	environment = saved_environment(NULL);
	if (i + 1 < size)
		return (parse_cmd(environment, cmds + i, FALSE));
	else
		return (cmds_parsed[i] = parse_cmd(environment, cmds + i, TRUE));
}

t_cmd	**get_cmds_parsed(t_env *environment, char **cmds)
{
	t_cmd	**cmds_parsed;
	size_t	size;
	size_t	i;

	size = 0;
	while (cmds[size])
		size++;
	cmds_parsed = db_malloc(sizeof(t_cmd *) * (size + 1));
	if (!cmds_parsed)
		exit_erralloc(environment);
	i = 0;
	while (i < size)
	{
		cmds_parsed[i] = _get_cmd_parsed(cmds, cmds_parsed, i, size);
		if (!cmds_parsed[i])
			return (ft_free_cmds_parsed(cmds_parsed), db_freetab(cmds), NULL);
		if (!cmds_parsed[i]->args \
		|| !(cmds_parsed[i]->args[0]) \
		|| !(cmds_parsed[i]->args[0][0]))
			cmds_parsed[i]->redirect->to_execute = FALSE;
		cmds_parsed[i]->pid = -1;
		i++;
	}
	cmds_parsed[i] = NULL;
	return (cmds_parsed);
}
