/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 10:45:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 11:04:17 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strinset(const char *str, const char *set, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	notfound;

	if (!str || !set || !(*set))
		return (1);
	if (len == 0)
		len = ft_strlen(str);
	notfound = ft_strlen(set);
	i = 0;
	while (str[i] && i < len)
	{
		j = 0;
		while (set[j])
		{
			if (str[i] == set[j])
				break ;
			j++;
		}
		if (j == notfound)
			return (0);
		i++;
	}
	return (1);
}
