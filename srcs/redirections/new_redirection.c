/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:11:19 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 18:48:40 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_redirection(t_env *environment, char *line, \
	t_redirect *redirect, int redirection_type)
{
	size_t	i;
	char	*start;
	char	*end;
	char	*new;

	i = 0;
	while (line[i] == SEPARATOR)
		i++;
	if (!redirection_check_syntax(environment, line + i, redirect))
		return (0);
	start = line + i;
	while (line[i] > 0 || line[i] == QUOTES)
		i++;
	end = line + i;
	new = db_strndup(start, end - start);
	remove_quote_token_line(new);
	ft_memmove(line, line + i, ft_strlen(line + i) + 1);
	redirect->lst = redirection_lstaddback(redirect->lst, new, \
	redirection_type);
	return (1);
}
