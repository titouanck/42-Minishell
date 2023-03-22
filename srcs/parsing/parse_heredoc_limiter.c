/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc_limiter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:32:45 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:45:16 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_replace_dollar_sign(char *c)
{
	if (*c == '$')
		*c = NOTAVARKEY;
}

void	parse_heredoc_limiter(t_env *environment, char *line)
{
	size_t	i;

	if (line[0] == '<' && line[1] == '<')
	{
		environment->limiter_between_quotes = 0;
		i = 2;
		while (ft_iswhitespace(line[i]))
			i++;
		if (line[i] == '\'' || line[i] == '\"')
			environment->limiter_between_quotes = 1;
		if (line[i] == '\'')
			while (line[++i] && line[i] != '\'')
				_replace_dollar_sign(line + i);
		else if (line[i] == '\"')
			while (line[++i] && line[i] != '\"')
				_replace_dollar_sign(line + i);
		else
		{
			while (line[i] && line[i] != '\"' && line[i] != '\'')
			{
				_replace_dollar_sign(line + i);
				i++;
			}
		}	
	}
}
