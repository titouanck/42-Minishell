/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:34:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 17:20:23 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_export_element(t_env *environment, \
	char *key, char *value)
{
	t_env	*elem;
	char	*tmp;
	
	elem = environment;
	if (!elem)
	{
		db_free(key);
		db_free(value);
		return ;
	}
	if (!key)
	{
		db_free(value);
		return ;
	}
	elem = elem->next;
	while (elem)
	{
		if (ft_strcmp(elem->key, key) == 0)
		{
			db_free(key);
			elem->exported = 1;
			if (value)
			{
				tmp = elem->value;
				elem->value = value;
				db_free(tmp);
			}
			return ;
		}
		elem = elem->next;
	}
	if (value)
		env_lstaddback(environment, key, value, 1);
	else
		db_free(key);
}

static void	_noarg(t_env *environment)
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

int	_too_many_args(char **args, int i)
{
	if (ft_len(args) > i + 1)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_returnval = 1;
		return (1);
	}
	return (0);
}

void	ftbuiltin_cd(t_env *environment, char **args)
{
	char	*cwd;
	char	*key;
	char	*value;
	size_t	i;
	int		r;

	i = 1;
	key = db_strdup("OLDPWD");
	value = getcwd(NULL, 0);
	if (args && args[0] && args[i] && ft_strncmp(args[i], "-", 1) == 0)
	{
		if (ft_strlen(args[i]) >= 2)
			args[i][2] = '\0';
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[i], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_returnval = 2;
		db_free(key);
		db_free(value);
		return ;
	}
	if (_too_many_args(args, i))
	{
		db_free(key);
		db_free(value);
		return ;
	}
	if (args && args[0] && !args[i])
		_noarg(environment);
	else if (args && args[0] && args[i])
	{
		r = chdir(args[i]);
		if (r == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(args[i]);
			g_returnval = 1;
		}
		else
			g_returnval = 0;
	}
	else
		g_returnval = 1;
	cwd = getcwd(NULL, 0);
	if (ft_strcmp(cwd, value) != 0)
		_export_element(environment, key, value);
	else
	{
		db_free (key);
		db_free(value);
	}
	db_free(cwd);
	key = db_strdup("PWD");
	value = getcwd(NULL, 0);
	_export_element(environment, key, value);
}
