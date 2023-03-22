/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:17:52 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:55:42 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_selection_action(char *c, \
	int *single_open, int *double_open)
{
	if (*single_open)
	{
		if (*c == '\'')
			*single_open = FALSE;
	}
	else if (*double_open)
	{
		if (*c == '\"')
			*double_open = FALSE;
	}
	else
	{
		if (*c == '\'')
			*single_open = TRUE;
		else if (*c == '\"')
			*double_open = TRUE;
		else if (*c == '|')
			*c = PIPECHAR;
	}
}

static int	_check_doublons(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == PIPECHAR && line[i + 1] == PIPECHAR)
			return (0);
		i++;
	}
	return (1);
}

static char	*_format_line_and_init(char **ptr, \
	int *single_open, int *double_open)
{
	char	*line;

	if (!(*ptr))
		return (NULL);
	line = ft_strip(*ptr);
	*single_open = FALSE;
	*double_open = FALSE;
	return (line);
}

char	**split_cmds(t_env *environment, char **ptr)
{
	char	*line;
	size_t	i;
	int		single_open;
	int		double_open;
	char	**cmds;

	line = _format_line_and_init(ptr, &single_open, &double_open);
	if (!line)
		return (NULL);
	i = 0;
	if (line[i] == '|')
		return (ft_syntaxerror(environment, "|"), NULL);
	while (line[i])
	{
		_selection_action(line + i, &single_open, &double_open);
		i++;
	}
	if (i > 0 && line[i - 1] == PIPECHAR)
		return (ft_syntaxerror(environment, "|"), NULL);
	if (!_check_doublons(line))
		return (ft_syntaxerror(environment, "|"), NULL);
	cmds = db_split(line, PIPECHAR);
	if (!cmds)
		exit_erralloc(environment);
	return (cmds);
}
