/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 13:47:57 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/02 17:00:32 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_command_not_found(char **args)
{
	g_returnval = 127;
}

char	*_locate_file(char **path, char *arg)
{
	char	*filepath;
	size_t	i;

	filepath = ft_strdup(arg);
	if (!filepath)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	if (access(filepath, F_OK) == 0)
	{
		if (access(filepath, X_OK) == 0)
			return (filepath);
		else
			return (ft_putstr_fd("minishell: ", 2), perror(arg), free(filepath), NULL);
	}
	free(filepath);
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		filepath = ft_strrjoin(path[i], "/", arg);
		if (!filepath)
			return (ft_putstr_fd(ERRALLOC, 2), NULL);
		if (access(filepath, F_OK) == 0)
		{
			if (access(filepath, X_OK) == 0)
				return (filepath);
			else
				return (ft_putstr_fd("minishell: ", 2), perror(arg), free(filepath), NULL);
		}
		free(filepath);
		i++;
	}
	return (ft_putstr_fd("minishell: ", 2), perror(arg), NULL);
}

int	execute_cmd(t_env *environment, char **args)
{
	char	**envp;
	char	**path;
	char	*filepath;
	char	*errmsg;

	if (!args)
		return (_command_not_found(args), 0);
	if (!args[0])
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		return (_command_not_found(args), 0);
	}
	envp = format_environment(environment);
	path = get_path(envp);
	filepath = _locate_file(path, args[0]);
	if (filepath)
	{
		execve(filepath, args, envp);
		free(filepath);
		filepath = NULL;
		errmsg = ft_strjoin("minishell: ", args[0]);
		perror(errmsg);
		if (errmsg)
			free(errmsg);
	}
	if (envp)
		free_tabstr(envp);
	if (path)
		free_tabstr(path);
	return (_command_not_found(args), 0);
}
