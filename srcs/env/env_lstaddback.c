/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lstaddback.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:29:12 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 17:12:39 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_lstaddback(t_env *env, char *key, char *value, int exported)
{
	if (!env)
		return (0);
	while (env->next)
		env = env->next;
	env->next = db_malloc(sizeof(t_env));
	if (!(env->next))
		exit_erralloc(NULL);
	env = env->next;
	env->key = key;
	env->value = value;
	env->exported = exported;
	env->next = NULL;
	return (1);
}