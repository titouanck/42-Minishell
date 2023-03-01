/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:34:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	_export_element(t_env *environment, \
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
		env_lstaddback(environment, key, value, 1);
	else
		free(key);
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

void	ftbuiltin_cd(t_env *environment, char **args)
{
	char	*err_str;
	char	*key;
	char	*value;
	int		r;

	key = ft_strdup("OLDPWD");
	value = getcwd(NULL, 0);
	_export_element(environment, key, value);
	if (args && args[0] && !args[1])
		_noarg(environment);
	else if (args && args[0] && args[1])
	{
		r = chdir(args[1]);
		if (r == 1)
		{
			err_str = ft_strjoin("minishell: cd: ", args[1]);
			if (!err_str)
				ft_putstr_fd(ERRALLOC, 2);
			perror(err_str);
			free(err_str);
			g_returnval = 1;
		}
		else
			g_returnval = 0;
	}
	else
		g_returnval = 1;
	key = ft_strdup("PWD");
	value = getcwd(NULL, 0);
	_export_element(environment, key, value);
}
