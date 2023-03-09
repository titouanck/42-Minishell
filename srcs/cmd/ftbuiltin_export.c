/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:26:08 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_export_noarg(t_env *environment)
{
	while (environment)
	{
		if (environment->key)
		{
			write(1, "export ", 7);
			write(1, environment->key, ft_strlen(environment->key));
			write(1, "=", 1);
			write(1, environment->value, ft_strlen(environment->value));
			write(1, "\n", 1);
		}
		environment = environment->next;
	}
}

static void	_export_element(t_env *environment, \
	char *key, char *value, int append)
{
	t_env	*elem;
	char	*tmp;
	
	elem = environment;
	if (!key || !elem)
		return ;
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
				if (append)
				{
					elem->value = ft_strjoin(elem->value, value);
					db_free(value);
				}
				else
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

static int	_export_key(char *arg, char *key, size_t i)
{
	char	*errmsg;

	(void) i;
	if (!key)
	{
		g_returnval = 1;
		return (0);
	}
	if (ft_isdigit(*key) \
	|| (!ft_strinset(key, VARNAMESET, ft_strlen(key) - 1)) \
	|| (*key == '+' || !ft_strinset(key + (ft_strlen(key) - 1), VARNAMESET "+", 1)))
	{
		errmsg = ft_strrjoin("minishell: export: `", arg, "': not a valid identifier\n");
		ft_putstr_fd(errmsg, 2);
		if (errmsg)
			db_free(errmsg);
		g_returnval = 1;
		return (0);
	}
	return (1);
}

static char	*_export_value(t_env *environment, char *arg, char **key, int append)
{
	char	*value;

	value = NULL;
	if (arg[0] == '\0')
		_export_element(environment, *key, NULL, append);
	else
	{
		value = ft_substr(arg, 1, ft_strlen(arg + 1));
		if (!value)
		{
			db_free(*key);
			*key = NULL;
			g_returnval = 1;
			return (NULL);
		}
		_export_element(environment, *key, value, append);
	}
	return (value);
}

void	ftbuiltin_export(t_env *environment, char **args)
{
	char	*key;
	size_t	j;
	size_t	i;
	char	*arg;

	if (!args[1])
		_export_noarg(environment);
	else
	{
		j = 1;
		while (args[j])
		{
			arg = args[j];
			i = 0;
			while (arg[i] && arg[i] != '=')
				i++;
			key = ft_substr(arg, 0, i);
			if (!_export_key(arg, key, i))
				db_free(key);
			else
			{
				arg = arg + i;
				if (*key && key[ft_strlen(key) - 1] == '+')
				{
					key[ft_strlen(key) - 1] = '\0';
					_export_value(environment, arg, &key, TRUE);
				}
				else
					_export_value(environment, arg, &key, FALSE);
			}
			j++;
		}
	}
	g_returnval = 0;
}

