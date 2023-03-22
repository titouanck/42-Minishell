/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote_token_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:15:02 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 14:15:14 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quote_token_line(char *line)
{
	size_t	i;

	if (!line)
		return ;
	i = 0;
	while (line[i])
	{
		if (line[i] == QUOTES)
		{
			ft_memmove(line + i, line + i + 1, ft_strlen(line + i + 1) + 1);
			i--;
		}
		i++;
	}
}
