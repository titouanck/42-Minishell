/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:26:47 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/06 14:26:58 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*new;
	size_t	i;

	if (!s1)
		return (NULL);
	if (ft_strlen(s1) < n)
		n = ft_strlen(s1);
	new = malloc(sizeof(char) * (n + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
