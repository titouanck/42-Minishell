/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:49:23 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 12:58:28 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*_with_readline(t_env *environment)
{
	char	*line;

	line = readline(environment->prompt);
	dynamic_memory_address_db('+', line);
	return (line);
}

char	*_without_readline(void)
{
	char	*line;

	line = get_next_line(0);
	if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}

char	*new_prompt(t_env *environment)
{
	char		*line;

	if (use_readline())
	{
		default_signal_tty();
		line = _with_readline(environment);
	}
	else
	{
		default_signal_notty();
		line = _without_readline();
	}
	return (line);
}
