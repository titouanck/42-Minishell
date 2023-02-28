/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:25:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 10:52:40 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strinsert(char *str, char *insertion, int index)
{
	char	*new;
	int		i;
	int		j;
 
	if (!str && !insertion)
		return (NULL);
	else if (!str)
		return (ft_strdup(insertion));
	else if (!insertion)
		return (ft_strdup(str));
	i = 0;
	while (str[i] && i < index)
		i++;
	new = malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(insertion) + 1));
	if (!new)
		return (NULL);
	j = 0;
	if (str[i])
	{
		while (str[j] && j < index)
		{
			new[j] = str[j];
			j++;
		}
	}
	i = 0;
	while (insertion[i])
	{
		new[j] = insertion[i];
		j++;
		i++;
	}
	i = index;
	while (str[i])
	{
		new[j] = str[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}
