/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:23:55 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parsing(char **line)
{
	char	**cmds;
	size_t	i;
	int		r;
	size_t	size;

	cmds = split_cmds(line);
	if (!cmds)
		return ;
	size = 0;
	while (cmds[size])
		size++;
	i = 0;
	while (cmds[i])
	{
		while (1)
		{
			r = change_local_variables(cmds[i], size);
			if (r == -1)
				break ;
			else if (r != 1)
				break ;
		}
		if (r != -1)
			ft_strip(cmds[i]);
		else
		{
			ft_freetab(cmds + i);
			cmds[i] = NULL;
			break ;
		}
		i++;
	}
	if (cmds && *cmds)
	{
		pipex(cmds);
		default_signal_behavior();
	}
	else
		ft_freetab(cmds);
}
