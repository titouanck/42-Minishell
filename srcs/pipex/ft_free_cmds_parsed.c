/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_cmds_parsed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:01:54 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 15:02:08 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_cmds_parsed(t_cmd **tab)
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		if (tab[i]->args)
			db_freetab(tab[i]->args);
		if (tab[i]->redirect)
			ft_free_redirect(tab[i]->redirect);
		db_free(tab[i]);
		i++;
	}
	db_free(tab);
}
