/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 13:47:57 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 12:08:42 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*_find_without_path(char *arg)
{
	char	*filepath;

	filepath = db_strdup(arg);
	if (!filepath)
		exit_erralloc(NULL);
	if (access(filepath, F_OK) == 0)
	{
		if (access(filepath, X_OK) == 0)
			return (filepath);
		else
		{
			if (errno == 13)
				g_returnval = 126;
			return (ft_putstr_fd("minishell: ", 2), perror(arg), \
			db_free(filepath), NULL);
			return (ft_putstr_fd("minishell: ", 2), perror(arg), \
			db_free(filepath), NULL);
		}
	}
	db_free(filepath);
	if (ft_strchr(arg, '/'))
		return (minishell_error(arg, ": No such file or directory\n"), NULL);
	else
		return (minishell_error(arg, ": command not found\n"), NULL);
}

static char	*_find_with_path(char **path, char *arg)
{
	char	*filepath;
	size_t	i;

	i = 0;
	while (path[i])
	{
		filepath = db_strrjoin(path[i], "/", arg);
		if (!filepath)
			exit_erralloc(NULL);
		if (access(filepath, F_OK) == 0)
		{
			if (access(filepath, X_OK) == 0)
				return (filepath);
			if (errno == 13)
				g_returnval = 126;
			return (ft_putstr_fd("minishell: ", 2), perror(arg), \
			db_free(filepath), NULL);
		}
		db_free(filepath);
		i++;
	}
	if (ft_strchr(arg, '/'))
		return (minishell_error(arg, ": No such file or directory\n"), NULL);
	else
		return (minishell_error(arg, ": command not found\n"), NULL);
}

static char	*_find_cmd_pathname(char **path, char *arg)
{
	if (ft_strcmp(arg, ".") == 0 || ft_strcmp(arg, "..") == 0)
		return (minishell_error(arg, ": command not found\n"), NULL);
	else if (ft_strncmp(arg, "/", 1) == 0 \
	|| ft_strncmp(arg, "./", 2) == 0 \
	|| ft_strncmp(arg, "../", 3) == 0)
		return (_find_without_path(arg));
	else if (!path)
		return (minishell_error(arg, ": No such file or directory\n"), NULL);
	else
		return (_find_with_path(path, arg));
}

static void	_try_to_execute(char **args, char **envp, char *filepath)
{
	cmd_signal_behavior();
	execve(filepath, args, envp);
	default_signal_behavior();
	db_free(filepath);
	filepath = NULL;
	if (errno == 13)
	{
		g_returnval = 126;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		perror(args[0]);
		g_returnval = errno;
	}
}

int	execute_cmd(t_env *environment, char **args)
{
	char	**envp;
	char	**path;
	char	*filepath;

	g_returnval = 127;
	if (!args || !args[0])
		return (ft_putstr_fd("minishell: : command not found\n", 2), 0);
	if (environment)
		envp = format_environment(environment);
	else
		envp = NULL;
	path = get_path(envp);
	filepath = _find_cmd_pathname(path, args[0]);
	if (filepath)
		_try_to_execute(args, envp, filepath);
	if (envp)
		db_freetab(envp);
	if (path)
		db_freetab(path);
	return (0);
}
