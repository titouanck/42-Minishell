/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 09:47:45 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/24 16:12:19 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strip(char *str)
{
	size_t	i;

	if (!str)
		return (NULL);
	i = ft_strlen(str);
	if (i == 0)
		return (str);
	i = i - 1;
	while (i > 0 && (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r')))
		i--;
	str[i + 1] = '\0';
	i = 0;
	while (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r'))
		i++;
	if (i > 0)
		ft_memmove(str, str + i, ft_strlen(str + i) + 1);
	return (str);
}
