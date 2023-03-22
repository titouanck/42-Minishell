/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_environment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:08:28 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 17:08:34 by tchevrie         ###   ########.fr       */
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
