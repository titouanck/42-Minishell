/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:34:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 12:47:13 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_update_existing_key_env(t_env *env_elem, \
	char *key, char *value, int append)
{
	char	*tmp;

	db_free(key);
	env_elem->exported = 1;
	if (value)
	{
		tmp = env_elem->value;
		if (append)
		{
			env_elem->value = db_strjoin(env_elem->value, value);
			db_free(value);
		}
		else
			env_elem->value = value;
		db_free(tmp);
	}
}

void	update_environment(t_env *environment, \
	char *key, char *value, int append)
{
	t_env	*env_elem;

	env_elem = environment;
	if (!env_elem || !key)
	{
		db_free(key);
		db_free(value);
		return ;
	}
	env_elem = env_elem->next;
	while (env_elem)
	{
		if (ft_strcmp(env_elem->key, key) == 0)
		{
			_update_existing_key_env(env_elem, key, value, append);
			return ;
		}
		env_elem = env_elem->next;
	}
	if (value)
		env_lstaddback(environment, key, value, 1);
	else
		db_free(key);
}

static void	_update_existing_key_local(t_env *env_elem, \
	char *key, char *value, int append)
{
	char	*tmp;

	db_free(key);
	if (value)
	{
		tmp = env_elem->value;
		if (append)
		{
			env_elem->value = db_strjoin(env_elem->value, value);
			db_free(value);
		}
		else
			env_elem->value = value;
		db_free(tmp);
	}
}

void	update_local_variables(t_env *environment, \
	char *key, char *value, int append)
{
	t_env	*env_elem;

	env_elem = environment;
	if (!env_elem || !key)
	{
		db_free(key);
		db_free(value);
		return ;
	}
	env_elem = env_elem->next;
	while (env_elem)
	{
		if (ft_strcmp(env_elem->key, key) == 0)
		{
			_update_existing_key_local(env_elem, key, value, append);
			return ;
		}
		env_elem = env_elem->next;
	}
	if (value)
		env_lstaddback(environment, key, value, 0);
	else
		db_free(key);
}
