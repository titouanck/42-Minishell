/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:17:52 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/16 19:25:45 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_actions_singlequoteopen(char c, \
	int *single_quote_open)
{
	if (c == '\'')
		*single_quote_open = FALSE;
}

static void	_actions_doublequoteopen(char c, \
	int *double_quote_open)
{
	if (c == '\"')
		*double_quote_open = FALSE;
}

static void	_actions_default(char *c, \
	int *single_quote_open, int *double_quote_open)
{
	if (*c == '\'')
		*single_quote_open = TRUE;
	else if (*c == '\"')
		*double_quote_open = TRUE;
	else if (*c == '|')
		*c = PIPECHAR;
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

char	**split_cmds(t_env *environment, char **ptr)
{
	char	*line;
	size_t	i;
	int		single_quote_open;
	int		double_quote_open;
	char	**cmds;

	line = ft_strip(*ptr);
	if (!line)
		return (NULL);
	single_quote_open = FALSE;
	double_quote_open = FALSE;
	i = 0;
	if (line[i] == '|')
		return (ft_syntaxerror(environment, "|"), NULL);
	while (line[i])
	{
		if (single_quote_open)
			_actions_singlequoteopen(line[i], &single_quote_open);
		else if (double_quote_open)
			_actions_doublequoteopen(line[i], &double_quote_open);
		else
			_actions_default(line + i, \
			&single_quote_open, &double_quote_open);
		i++;
	}
	if (i > 0 && line[i - 1] == PIPECHAR)
		return (ft_syntaxerror(environment, "|"), NULL);
	if (!_check_doublons(line))
		return (ft_syntaxerror(environment, "|"), NULL);
	cmds = db_split(line, PIPECHAR);
	if (!cmds)
		ft_putstr_fd(ERRALLOC, 2);
	return (cmds);
}
