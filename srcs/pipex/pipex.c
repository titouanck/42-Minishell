/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:07:04 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/27 17:52:37 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	revert_to_default_fds(int defaultfd[2])
{
	dup2(defaultfd[0], 0);
	dup2(defaultfd[1], 1);
}

void	pipex(t_env *environment, char **cmds)
{
	int		defaultfd[2];
	int		pipefd[2];
	int		middle_cmds;
	size_t	size;
	size_t	cmdnbr;

	defaultfd[0] = dup(0);
	defaultfd[1] = dup(1);
	env_lstaddback(environment, ft_strdup("@!stdincp"), ft_itoa(defaultfd[0]), 0);
	env_lstaddback(environment, ft_strdup("@!stdoutcp"), ft_itoa(defaultfd[1]), 0);
	if (defaultfd[0] == -1 || defaultfd[1] == -1)
	{
		ftbuiltin_unset_element(environment, "@!stdincp");
		ftbuiltin_unset_element(environment, "@!stdoutcp");
		return ;
	}
	size = 0;
	while (cmds[size])
		size++;
	if (size > 1)
	{
		if (!first_child(environment, pipefd, cmds))
		{
			revert_to_default_fds(defaultfd);
			close(defaultfd[0]);
			close(defaultfd[1]);
			ftbuiltin_unset_element(environment, "@!stdincp");
			ftbuiltin_unset_element(environment, "@!stdoutcp");
			return ;
		}	
		middle_cmds = size - 2;
		cmdnbr = 1;
		while (middle_cmds-- > 0)
		{
			revert_to_default_fds(defaultfd);
			middle_child(environment, pipefd, cmds, cmdnbr++);
		}
	}
	revert_to_default_fds(defaultfd);
	last_child(environment, pipefd, cmds, size);
	while (1)
		if (wait(NULL) <= 0)
			break ;
	revert_to_default_fds(defaultfd);
	close(defaultfd[0]);
	close(defaultfd[1]);
	ftbuiltin_unset_element(environment, "@!stdincp");
	ftbuiltin_unset_element(environment, "@!stdoutcp");
}
