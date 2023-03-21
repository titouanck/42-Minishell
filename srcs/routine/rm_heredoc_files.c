/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_heredoc_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:24:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 20:01:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_remove_in_child(char **heredoc_files, size_t i)
{
	char	*tmp;
	char	**cmd;

	tmp = ft_strjoin("/usr/bin/rm rm -f ", heredoc_files[i]);
	cmd = ft_split(tmp, ' ');
	free(tmp);
	if (cmd)
	{
		execve(cmd[0], cmd + 1, NULL);
		ft_freetab(cmd);
	}
	tmp = ft_strjoin("/bin/rm rm -f ", heredoc_files[i]);
	cmd = ft_split(tmp, ' ');
	free(tmp);
	if (cmd)
	{
		execve(cmd[0], cmd + 1, NULL);
		ft_freetab(cmd);
	}
	ft_freetab(heredoc_files);
	exit(127);
}

void	rm_heredoc_files(t_env *environment)
{
	char	**heredoc_files;
	size_t	i;
	pid_t	pid;

	heredoc_files = ft_split(environment->heredoc_files, '|');
	db_free(environment->heredoc_files);
	environment->heredoc_files = NULL;
	if (!heredoc_files)
		return ;
	i = 0;
	while (heredoc_files[i])
	{
		pid = fork();
		if (pid == 0)
			_remove_in_child(heredoc_files, i);
		else
			wait(NULL);
		i++;
	}
	ft_freetab(heredoc_files);
}
