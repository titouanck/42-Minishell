/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:34:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:26:38 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_export_element(\
	char *key, char *value)
{
	t_env	*elem;
	char	*tmp;
	
	elem = environment;
	if (!elem)
	{
		free(key);
		free(value);
		return ;
	}
	if (!key)
	{
		free(value);
		return ;
	}
	elem = elem->next;
	while (elem)
	{
		if (ft_strcmp(elem->key, key) == 0)
		{
			free(key);
			elem->exported = 1;
			if (value)
			{
				tmp = elem->value;
				elem->value = value;
				free(tmp);
			}
			return ;
		}
		elem = elem->next;
	}
	if (value)
		env_lstaddback(key, value, 1);
	else
		free(key);
}

static void	_noarg()
{
	char	*err_str;
	char	*home;
	int		r;
	
	home = get_value_by_key("HOME");
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_returnval = 1;
		return ;
	}
	r = chdir(home);
	if (r == -1)
	{
		err_str = ft_strjoin("minishell: cd: ", home);
		if (!err_str)
			ft_putstr_fd(ERRALLOC, 2);
		perror(err_str);
		free(err_str);
		g_returnval = 1;
	}
	else
		g_returnval = 0;
}

void	ftbuiltin_cd(char **args)
{
	char	*cwd;
	char	*err_str;
	char	*key;
	char	*value;
	int		r;

	key = ft_strdup("OLDPWD");
	value = getcwd(NULL, 0);
	if (args && args[0] && !args[1])
		_noarg();
	else if (args && args[0] && args[1])
	{
		r = chdir(args[1]);
		if (r == -1)
		{
			err_str = ft_strjoin("minishell: cd: ", args[1]);
			if (!err_str)
				ft_putstr_fd(ERRALLOC, 2);
			perror(err_str);
			free(err_str);
			free(key);
			free(value);
			g_returnval = 1;
		}
		else
			g_returnval = 0;
	}
	else
		g_returnval = 1;
	cwd = getcwd(NULL, 0);
	if (ft_strcmp(cwd, value) != 0)
		_export_element(key, value);
	else
	{
		free (key);
		free(value);
	}
	free(cwd);
	key = ft_strdup("PWD");
	value = getcwd(NULL, 0);
	_export_element(key, value);
}
