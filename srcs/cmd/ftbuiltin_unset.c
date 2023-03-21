/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 18:15:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 13:37:06 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ftbuiltin_unset_element(t_env *environment, char *arg)
{
	t_env	*elem;
	t_env	*tmp;
	
	elem = environment;
	if (!arg || !elem)
		return ;
	while (elem && elem->next)
	{
		if (ft_strcmp(elem->next->key, arg) == 0)
		{
			tmp = elem->next;
			elem->next = elem->next->next;
			db_free(tmp->key);
			db_free(tmp->value);
			db_free(tmp);
			return ;
		}
		elem = elem->next;
	}
}

void	ftbuiltin_unset(t_env *environment, char **args)
{
	size_t	i;
	char	*arg;

	if (args && args[0] && args[1] && ft_strncmp(args[1], "-", 1) == 0)
	{
		if (ft_strlen(args[1]) >= 2)
			args[1][2] = '\0';
		ft_putstr_fd("minishell: unset: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_returnval = 2;
		return ;
	}
	g_returnval = 0;
	i = 1;
	while (args[i])
	{
		arg = args[i];
		if (!(*arg) || ft_isdigit(*arg) || !ft_strinset(arg, VARNAMESET, ft_strlen(arg)))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("\': not a valid identifier\n", 2);
			g_returnval = 1;
			return ;
		}
		else
			ftbuiltin_unset_element(environment, arg);
		i++;
	}
}
