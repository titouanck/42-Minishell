/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 13:47:57 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/14 15:03:12 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*_locate_file(char **path, char *arg)
{
	char	*filepath;
	size_t	i;

	if (ft_strncmp(arg, "/", 1) == 0 || ft_strncmp(arg, "./", 2) == 0 || ft_strncmp(arg, "../", 3) == 0)
	{
		filepath = db_strdup(arg);
		if (!filepath)
			return (ft_putstr_fd(ERRALLOC, 2), NULL);
		if (access(filepath, F_OK) == 0)
		{
			if (access(filepath, X_OK) == 0)
				return (filepath);
			else
			{
				if (errno == 13)
					g_returnval = 126;
				return (ft_putstr_fd("minishell: ", 2), perror(arg), db_free(filepath), NULL);
			}
		}
		db_free(filepath);
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": No such file or directory\n", 2), NULL);
	}
	if (!path)
	{
		if (ft_strchr(arg, '/'))
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": No such file or directory\n", 2), NULL);
		else
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": command not found\n", 2), NULL);
	}
	i = 0;
	while (path[i])
	{
		filepath = db_strrjoin(path[i], "/", arg);
		if (!filepath)
			return (ft_putstr_fd(ERRALLOC, 2), NULL);
		if (access(filepath, F_OK) == 0)
		{
			if (access(filepath, X_OK) == 0)
				return (filepath);
			else
			{
				if (errno == 13)
					g_returnval = 126;
				return (ft_putstr_fd("minishell: ", 2), perror(arg), db_free(filepath), NULL);
			}
		}
		db_free(filepath);
		i++;
	}
	if (ft_strchr(arg, '/'))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": No such file or directory\n", 2), NULL);
	else
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(arg, 2), ft_putstr_fd(": command not found\n", 2), NULL);

}

int	execute_cmd(t_env *environment, char **args)
{
	char	**envp;
	char	**path;
	char	*filepath;

	g_returnval = 127;
	if (!args || !args[0])
		return (ft_putstr_fd("minishell: : command not found\n", 2), 0);
	envp = format_environment(environment);
	path = get_path(envp);
	filepath = _locate_file(path, args[0]);
	if (filepath)
	{
		cmd_signal_behavior();
		execve(filepath, args, envp);
		default_signal_behavior();
		db_free(filepath);
		filepath = NULL;
		if (errno == 13)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			g_returnval = 126;
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			perror(args[0]);
		}
	}
	if (envp)
		db_freetab(envp);
	if (path)
		db_freetab(path);
	return (0);
}
