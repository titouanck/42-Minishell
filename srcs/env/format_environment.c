/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:09:38 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/31 02:53:58 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	_format_element(t_env *environment, t_env *elem, char **env, size_t i)
{
	env[i] = db_strrjoin(elem->key, "=", elem->value);
	if (!env[i])
		exit_erralloc(environment);
}

char	**format_environment(t_env *environment)
{
	t_env	*elem;
	size_t	size;
	size_t	i;
	char	**env;

	elem = environment->next;
	size = 0;
	while (elem && ++size)
		elem = elem->next;
	env = db_malloc(sizeof(char *) * (size + 1));
	if (!env)
		exit_erralloc(environment);
	i = 0;
	elem = environment->next;
	while (i < size && elem)
	{
		if (elem->exported == 1)
		{
			_format_element(environment, elem, env, i);
			i++;
		}
		elem = elem->next;
	}
	env[i] = NULL;
	return (env);
}
