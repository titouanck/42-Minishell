/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:23:21 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_format_cmd(t_env *environment, char **cmds, size_t i, size_t size)
{
	int		r;

	while (1)
	{
		r = change_local_variables(environment, cmds[i], size);
		if (r == -1)
			break ;
		else if (r != 1)
			break ;
	}
	if (r != -1)
		ft_strip(cmds[i]);
	else
	{
		db_freetab(cmds + i);
		cmds[i] = NULL;
		return (0);
	}
	return (1);
}

void	parsing(t_env *environment, char **line)
{
	char	**cmds;
	size_t	i;
	size_t	size;

	cmds = split_cmds(environment, line);
	if (!cmds)
		return ;
	size = 0;
	while (cmds[size])
		size++;
	size = 0;
	i = 0;
	while (cmds[i])
	{
		if (!_format_cmd(environment, cmds, i, size))
			break ;
		i++;
	}
	if (cmds && *cmds)
		pipex(environment, cmds);
	else
		db_freetab(cmds);
}
