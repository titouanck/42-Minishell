/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	last_child(t_env *environment, int pipefd[2], char **cmds, size_t size)
{
	pid_t	pid;
	char	**args;

	if (size > 1)
		close(pipefd[1]);
	args = parse_cmd(environment, cmds + size - 1);
	if (!args)
		return ;
	// if (parse_builtin(environment, args, cmds, size - 1))
	// {
	// 	if (size > 1)
	// 		close(pipefd[0]);
	// 	return ;
	// }
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		if (size > 1)
			close(pipefd[0]);
	}
	else if (pid == 0)
	{
		if (size > 1)
			dup2(pipefd[0], STDIN_FILENO);
		execute_cmd(environment, args);
		if (size > 1)
			close(pipefd[0]);
		free_tabstr(cmds);
		closing_the_program(environment);
		exit(g_returnval);
	}
	else
	{
		waitpid(pid, &g_returnval, 0);
		g_returnval = WEXITSTATUS(g_returnval);
		if (size > 1)
			close(pipefd[0]);
	}
	free_tabstr(args);
}
