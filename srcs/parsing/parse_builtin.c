/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:49:18 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 13:04:33 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_builtin(t_env *environment, char **args, \
	t_cmd **cmds, size_t cmdnbr)
{
	size_t	size;

	size = 0;
	while (cmds[size])
		size++;
	if (ft_strcmp(args[0], "echo") == 0)
		ftbuiltin_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		ftbuiltin_cd(environment, args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ftbuiltin_pwd(environment);
	else if (ft_strcmp(args[0], "export") == 0)
		ftbuiltin_export(environment, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		ftbuiltin_unset(environment, args);
	else if (ft_strcmp(args[0], "env") == 0)
		ftbuiltin_env(environment);
	else if (ft_strcmp(args[0], "exit") == 0)
		ftbuiltin_exit(environment, args, cmds, cmdnbr);
	else if (ft_strcmp(args[0], "dma_db") == 0)
		ftbuiltin_dma_db();
	else
		return (0);
	return (1);
}
