/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 14:18:07 by tchevrie         ###   ########.fr       */
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

static int	_actions_default(char *line, size_t i, \
	int *single_quote_open, int *double_quote_open)
{
	int	j;

	if (line[i] == '\'')
		*single_quote_open = TRUE;
	else if (line[i] == '\"')
		*double_quote_open = TRUE;
	else
		return (0);
	j = (int) i - 1;
	if (j == -1)
		return (1);
	while ((j >= 0 && ft_iswhitespace(line[j]) && (1 || printf("(%c)", line[j]))))
		j--;
	if (j >= 0 && line[j] != '>' && line[j] != '<')
		return (1);
	return (0);
}

int	presence_of_cmd(char *line)
{
	size_t	i;
	int		single_quote_open;
	int		double_quote_open;

	i = 0;
	single_quote_open = FALSE;
	double_quote_open = FALSE;
	if (!line)
		return (0);
	while (line[i])
	{
		if (single_quote_open)
			_actions_singlequoteopen(line[i], &single_quote_open);
		else if (double_quote_open)
			_actions_doublequoteopen(line[i], &double_quote_open);
		else
			if (_actions_default(line, i, \
			&single_quote_open, &double_quote_open))
				return (1);
		i++;
	}
	return (0);
}

void	parsing(t_env *environment, char **line)
{
	char	**cmds;
	size_t	i;
	int		r;
	size_t	size;

	cmds = split_cmds(line);
	if (!cmds)
		return ;
	size = 0;
	while (cmds[size])
		size++;
	size = 0;
	i = 0;
	while (cmds[i])
	{
		while (1)
		{
			r = change_local_variables(environment, cmds[i], size);
			if (r == -1)
				break ;
			else if (r != 1)
				break ;
		}
		if (r != -1)
			ft_strip(cmds[i]);
		else
		{
			ft_freetab(cmds + i);
			cmds[i] = NULL;
			break ;
		}
		i++;
	}
	if (cmds && *cmds)
	{
		pipex(environment, cmds);
		default_signal_behavior();
	}
	else
		ft_freetab(cmds);
}
