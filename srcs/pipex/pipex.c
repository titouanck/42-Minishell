/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:07:04 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pipex(t_env *environment, char **cmds)
{
	int		pipefd[2];
	int		middle_cmds;
	size_t	size;
	size_t	cmdnbr;

	size = 0;
	while (cmds[size])
		size++;
	if (size > 1)
	{
		if (!first_child(environment, pipefd, cmds))
			return ;
		middle_cmds = size - 2;
		cmdnbr = 1;
		while (middle_cmds-- > 0)
			middle_child(environment, pipefd, cmds, cmdnbr++);
	}
	last_child(environment, pipefd, cmds, size);
	while (1)
		if (wait(NULL) <= 0)
			break ;
}
