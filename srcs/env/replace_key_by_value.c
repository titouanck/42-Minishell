/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:03:48 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 16:19:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_key_by_value(t_env *environment, char *line)
{
	char	*first_part;
	char	*second_part;
	int		question_mark;
	char	*var;
	char	*tmp;
	size_t	i;

	i = 0;
	while (line && line[i])
	{
		if (line[i] == VARKEY)
		{
			line[i] = '\0';
			first_part = ft_strdup(line);
			if (!first_part)
				return (free(line), NULL);
			question_mark = FALSE;
			if (line[i + 1] == '\?')
				question_mark = TRUE;
			if (question_mark)
				second_part = line + i + 2;
			else
			{
				second_part = line + i + 1;
				while (*second_part && ft_strinset(second_part, VARNAMESET, 1))
					second_part++;
			}
			var = second_part;
			second_part = ft_strdup(second_part);
			if (!second_part)
				return (free(line), free(first_part), NULL);
			*var = '\0';
			var = line + i + 1;
			if (question_mark)
			{
				i--;
				var = ft_itoa(g_returnval);
			}
			else
				var = get_value_by_key(environment, var);
			if (!var)
				var = "";
			tmp = ft_strrjoin(first_part, var, second_part);
			if (question_mark)
				free(var);
			free(line);
			free(first_part);
			free(second_part);
			line = tmp;
			if (!line)
				return (NULL);
			i--;
		}
		i++;
	}
	return (line);
}
