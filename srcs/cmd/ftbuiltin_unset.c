/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 18:15:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:29:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ftbuiltin_unset_element(char *arg)
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
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		elem = elem->next;
	}
}

void	ftbuiltin_unset(char **args)
{
	size_t	i;
	char	*arg;
	char	*errmsg;

	environment->g_returnval = 0;
	i = 1;
	while (args[i])
	{
		arg = args[i];
		if (*arg == '\0')
			return ;
		if (!ft_strinset(arg, VARNAMESET, ft_strlen(arg)))
		{
			errmsg = ft_strrjoin("minishell: unset: `", arg, "': not a valid identifier\n");
			ft_putstr_fd(errmsg, 2);
			if (errmsg)
				free(errmsg);
			environment->g_returnval = 1;
			return ;
		}
		else
			ftbuiltin_unset_element(arg);
		i++;
	}
}
