/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/27 17:18:44 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	first_child(t_env *environment, int pipefd[2], char **cmds)
{
	pid_t	pid;
	char	**args;

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 0);
	args = parse_cmd(environment, cmds);
	if (!args)
		return (1);
	dup2(pipefd[1], STDOUT_FILENO);
	if (parse_builtin(environment, args, cmds, 0))
	{
		close(pipefd[1]);
		return (1);
	}
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), free_tabstr(cmds), 0);
	else if (pid == 0)
	{
		close(pipefd[0]);
		execute_cmd(environment, args);
		close(pipefd[1]);
		free_tabstr(cmds);
		closing_the_program(environment);
		exit(g_returnval);
	}
	free_tabstr(args);
	return (1);
}
