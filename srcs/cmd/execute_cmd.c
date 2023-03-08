/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 13:47:57 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/06 17:26:28 by tchevrie         ###   ########.fr       */
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

	if (ft_strncmp(arg, "/", 1) == 0 || ft_strncmp(arg, ".", 1) == 0)
	{
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
	}
	if (!path)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": command not found\n", 2), NULL);
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
	return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": command not found\n", 2), NULL);
}

int	execute_cmd(t_env *environment, char **args)
{
	char	**envp;
	char	**path;
	char	*filepath;
	char	*errmsg;

	if (!args || !args[0])
		return (ft_putstr_fd("minishell: : command not found\n", 2), \
		_command_not_found(args), 0);
	envp = format_environment(environment);
	path = get_path(envp);
	filepath = _locate_file(path, args[0]);
	if (filepath)
	{
		execve(filepath, args, envp);
		free(filepath);
		filepath = NULL;
		if (errno == 13)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
		}
		else
		{
			errmsg = ft_strjoin("minishell: ", args[0]);
			perror(errmsg);
			if (errmsg)
				free(errmsg);
		}
	}
	if (envp)
		ft_freetab(envp);
	if (path)
		ft_freetab(path);
	return (_command_not_found(args), 0);
}
