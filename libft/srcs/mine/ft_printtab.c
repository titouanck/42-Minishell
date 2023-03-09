/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printtab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:54:03 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 13:55:41 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printtab(char **tab)
{
	size_t	i;

	if (!tab)
	{
		ft_printf(""BLUEBG"!tab"ENDCL);
		return ;
	}
	i = 0;
	while (tab[i])
	{
		ft_printf("%i: "BLUEBG"%s"ENDCL"\n", i, tab[i]);
		i++;
	}
}
