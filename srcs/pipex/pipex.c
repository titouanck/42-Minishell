/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:07:04 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 14:19:12 by tchevrie         ###   ########.fr       */
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
			ft_freetab(tab[i]->args);
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
		return (ft_putstr_fd(ERRALLOC, 2), ft_freetab(cmds), NULL);
	i = 0;
	while (i < size)
	{
		cmds_parsed[i] = parse_cmd(environment, cmds + i);
		if (!cmds_parsed[i])
			return (free_cmds_parsed(cmds_parsed), ft_freetab(cmds), NULL);
		if (!cmds_parsed[i]->args || !(cmds_parsed[i]->args[0]) || !(cmds_parsed[i]->args[0][0]))
			cmds_parsed[i]->redirect->to_execute = FALSE;
		
		ft_printf(REDBG);
		if (cmds_parsed[i]->redirect->infile)
			ft_printf("< %s\n", cmds_parsed[i]->redirect->infile);
		if (cmds_parsed[i]->redirect->outfile && cmds_parsed[i]->redirect->append == 0)
			ft_printf("> %s\n", cmds_parsed[i]->redirect->outfile);
		if (cmds_parsed[i]->redirect->outfile && cmds_parsed[i]->redirect->append == 1)
			ft_printf("> %s\n", cmds_parsed[i]->redirect->outfile);
		ft_printf(ENDCL);
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

	heredoc_signal_behavior();
	cmds_parsed = _get_cmds_parsed(environment, cmds);
	if (!cmds_parsed)
		return (0);
	ft_freetab(cmds);
	cmd_signal_behavior();
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
