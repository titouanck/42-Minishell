/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:07:04 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 17:25:01 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_call_childs(t_env *environment, t_cmd **cmds_parsed)
{
	size_t	cmdnbr;
	int		pipefd[2];

	cmdnbr = 0;
	if (cmds_parsed[1])
	{
		if (!first_child(environment, pipefd, cmds_parsed))
			return (0);
		cmdnbr++;
	}
	while (cmds_parsed[cmdnbr + 1])
	{
		if (!middle_child(environment, pipefd, cmds_parsed, cmdnbr))
		{
			while (wait(NULL) > 0)
				;
			return (ft_free_cmds_parsed(cmds_parsed), 0);
		}	
		cmdnbr++;
	}
	last_child(environment, pipefd, cmds_parsed, cmdnbr);
	return (1);
}

int	pipex(t_env *environment, char **cmds)
{
	t_cmd	**cmds_parsed;

	heredoc_signal();
	cmds_parsed = get_cmds_parsed(environment, cmds);
	if (!cmds_parsed)
		return (0);
	db_freetab(cmds);
	if (use_readline())
		cmd_signal_parent();
	if (!_call_childs(environment, cmds_parsed))
		return (0);
	while (1)
		if (wait(NULL) <= 0)
			break ;
	return (ft_free_cmds_parsed(cmds_parsed), 1);
}
