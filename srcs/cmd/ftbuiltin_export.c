/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:26:08 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 17:32:43 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_export_key(char *arg, char *key)
{
	if (!key || ft_isdigit(*key) \
	|| (!ft_strinset(key, VARNAMESET, ft_strlen(key) - 1)) \
	|| (*key == '+' \
	|| !ft_strinset(key + (ft_strlen(key) - 1), VARNAMESET "+", 1)))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		g_returnval = 1;
		return (0);
	}
	return (1);
}

static char	*_export_value(t_env *environment, \
	char *arg, char **key, int append)
{
	char	*value;

	value = NULL;
	if (arg[0])
	{
		value = db_substr(arg, 1, ft_strlen(arg + 1));
		if (!value)
			exit_erralloc(environment);
		update_environment(environment, *key, value, append);
	}
	else
		update_environment(environment, *key, NULL, append);
	return (value);
}

static void	_export_arg(t_env *environment, char *arg)
{
	char	*key;
	size_t	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (i > 0)
		key = db_substr(arg, 0, i);
	else
		key = NULL;
	if (!_export_key(arg, key))
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
}

void	ftbuiltin_export(t_env *environment, char **args)
{
	size_t	i;

	g_returnval = 0;
	if (!args[1] && environment)
		export_noarg(environment);
	else if (args && args[0] && args[1] && ft_strncmp(args[1], "-", 1) == 0)
	{
		if (ft_strlen(args[1]) >= 2)
			args[1][2] = '\0';
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_returnval = 2;
		return ;
	}
	else
	{
		i = 0;
		while (args[++i])
			_export_arg(environment, args[i]);
	}
}
