/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:34:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 18:47:49 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_error_handling(char **args, char *value)
{
	if (args && args[0] && args[1] && ft_strncmp(args[1], "-", 1) == 0)
	{
		if (ft_strlen(args[1]) >= 2)
			args[1][2] = '\0';
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_returnval = 2;
		db_free(value);
		return (0);
	}
	else if (ft_len(args) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_returnval = 1;
		db_free(value);
		return (0);
	}
	return (1);
}

static void	_without_arg(t_env *environment)
{
	char	*err_str;
	char	*home;
	int		r;

	home = get_value_by_key(environment, "HOME");
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_returnval = 1;
		return ;
	}
	r = chdir(home);
	if (r == -1)
	{
		err_str = db_strjoin("minishell: cd: ", home);
		if (!err_str)
			exit_erralloc(environment);
		perror(err_str);
		db_free(err_str);
		g_returnval = 1;
	}
	else
		g_returnval = 0;
}

static void	_with_arg(char **args)
{
	int		r;

	if (args && args[0] && args[1])
	{
		r = chdir(args[1]);
		if (r == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(args[1]);
			g_returnval = 1;
		}
		else
			g_returnval = 0;
	}
	else
		g_returnval = 1;
}

void	ftbuiltin_cd(t_env *environment, char **args)
{
	char	*cwd;
	char	*key;
	char	*value;

	value = getcwd(NULL, 0);
	dynamic_memory_address_db('+', value);
	if (!_error_handling(args, value))
		return ;
	else if (args && args[0] && !args[1])
		_without_arg(environment);
	else
		_with_arg(args);
	cwd = getcwd(NULL, 0);
	if (ft_strcmp(cwd, value) != 0)
	{
		key = db_strdup("OLDPWD");
		update_environment(environment, key, value, FALSE);
	}
	else
		db_free(value);
	db_free(cwd);
	key = db_strdup("PWD");
	value = getcwd(NULL, 0);
	dynamic_memory_address_db('+', value);
	update_environment(environment, key, value, FALSE);
}
