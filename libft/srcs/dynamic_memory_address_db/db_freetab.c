/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   db_freetab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:31:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/06 17:32:48 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	db_freetab(char **tab)
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		db_free(tab[i]);
		i++;
	}
	db_free(tab);
}
