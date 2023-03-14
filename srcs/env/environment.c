/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:29:12 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 09:15:54 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_environment(t_env *environment)
{
	t_env	*tmp;

	while (environment)
	{
		tmp = environment;
		environment = environment->next;
		if (tmp->key)
			db_free(tmp->key);
		if (tmp->value)
			db_free(tmp->value);
		db_free(tmp);
	}
}

char	**format_environment(t_env *environment)
{
	t_env	*elem;
	size_t	size;
	size_t	i;
	char	**env;

	if (!environment)
		return (NULL);
	elem = environment->next;
	size = 0;
	while (elem)
	{
		elem = elem->next;
		size++;
	}
	env = db_malloc(sizeof(char *) * (size + 1));
	if (!env)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	i = 0;
	elem = environment->next;
	while (i < size && elem)
	{
		env[i] = db_strrjoin(elem->key, "=", elem->value);
		if (!env[i])
			return (db_freetab(env), ft_putstr_fd(ERRALLOC, 2), NULL);
		elem = elem->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	*get_value_by_key(t_env *environment, char *key)
{
	t_env	*current;

	current = environment;
	if (!current)
		return (NULL);
	current = current->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int		env_lstaddback(t_env *env, char *key, char *value, int exported)
{
	if (!env)
		return (0);
	while (env->next)
		env = env->next;
	env->next = db_malloc(sizeof(t_env));
	if (!(env->next))
		return (ft_putstr_fd(ERRALLOC, 2), 0);
	env = env->next;
	env->key = key;
	env->value = value;
	env->exported = exported;
	env->next = NULL;
	return (1);
}
