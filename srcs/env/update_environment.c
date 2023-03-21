/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:34:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 18:47:49 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_environment(t_env *environment, t_env *env_elem, \
	char *key, char *value)
{
	char	*tmp;
	
	if (!env_elem || !key)
		return (db_free(key), db_free(value), 0);
	env_elem = env_elem->next;
	while (env_elem)
	{
		if (ft_strcmp(env_elem->key, key) == 0)
		{
			db_free(key);
			env_elem->exported = 1;
			if (value)
			{
				tmp = env_elem->value;
				env_elem->value = value;
				db_free(tmp);
			}
			return (1);
		}
		env_elem = env_elem->next;
	}
	if (value)
		return (env_lstaddback(environment, key, value, 1), 1);
	return (db_free(key), 0);
}
