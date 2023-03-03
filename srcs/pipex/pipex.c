/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:07:04 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/02 19:26:19 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmds_parsed(t_cmd **tab)
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		if (tab[i]->args)
			free_tabstr(tab[i]->args);
		if (tab[i]->redirect)
			free_redirect(tab[i]->redirect);
		free(tab[i]);
		i++;
	}
	free(tab);
}

static t_cmd	**_get_cmds_parsed(t_env *environment, char **cmds)
{
	t_cmd	**cmds_parsed;
	size_t	size;
	size_t	i;

	size = 0;
	while (cmds[size])
		size++;
	cmds_parsed = malloc(sizeof(t_cmd *) * (size + 1));
	if (!cmds_parsed)
		return (ft_putstr_fd(ERRALLOC, 2), free_tabstr(cmds), NULL);
	i = 0;
	while (i < size)
	{
		cmds_parsed[i] = parse_cmd(environment, cmds + i);
		if (!cmds_parsed[i])
			return (free_cmds_parsed(cmds_parsed), free_tabstr(cmds), NULL);
		if (!cmds_parsed[i]->args || !(cmds_parsed[i]->args[0]) || !(cmds_parsed[i]->args[0][0]))
			cmds_parsed[i]->redirect->to_execute = FALSE;
		i++;
	}
	cmds_parsed[i] = NULL;
	return (cmds_parsed);
}

int	pipex(t_env *environment, char **cmds)
{
	int		pipefd[2];
	size_t	cmdnbr;
	t_cmd	**cmds_parsed;

	cmds_parsed = _get_cmds_parsed(environment, cmds);
	if (!cmds_parsed)
		return (0);
	free_tabstr(cmds);
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
			while (1)
				if (wait(NULL) <= 0)
					break ;
			return (free_cmds_parsed(cmds_parsed), 0);
		}	
		cmdnbr++;
	}
	last_child(environment, pipefd, cmds_parsed, cmdnbr);
	while (1)
		if (wait(NULL) <= 0)
			break ;
	return (free_cmds_parsed(cmds_parsed), 1);
}
