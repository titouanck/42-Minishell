/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 12:48:16 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 16:09:49 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	_actions_singlequoteopen(char *line, \
	int *single_quote_open, size_t *i)
{
	if (line[(*i)] == '\'')
	{
		*single_quote_open = FALSE;
		ft_memmove(line + (*i), line + (*i) + 1, ft_strlen(line + (*i)));
		(*i)--;
	}
}

static void	_actions_doublequoteopen(char *line, \
	int *double_quote_open, size_t *i)
{
	if (line[(*i)] == '\"')
	{
		*double_quote_open = FALSE;
		ft_memmove(line + (*i), line + (*i) + 1, ft_strlen(line + (*i)));
		(*i)--;
	}
	else if (line[(*i)] == '$' && line[(*i) + 1] \
	&& ft_strinset(line + (*i) + 1, VARNAMESET "?", 1))
		line[(*i)] = VARKEY;
}

static void	_actions_default(char *line, \
	int *single_quote_open, int *double_quote_open, size_t *i)
{
	if (line[(*i)] == '\'')
	{
		*single_quote_open = TRUE;
		ft_memmove(line + (*i), line + (*i) + 1, ft_strlen(line + (*i)));
		(*i)--;
	}
	else if (line[(*i)] == '\"')
	{
		*double_quote_open = TRUE;
		ft_memmove(line + (*i), line + (*i) + 1, ft_strlen(line + (*i)));
		(*i)--;
	}
	else if (ft_iswhitespace(line[(*i)]))
		line[(*i)] = SEPARATOR;
	else if (line[(*i)] == '$' && line[(*i) + 1] \
	&& ft_strinset(line + (*i) + 1, VARNAMESET "?", 1))
		line[(*i)] = VARKEY;
	else if (line[(*i)] == '$' && line[(*i) + 1] \
	&& ft_strinset(line + (*i) + 1, "\"\'", 1))
		line[(*i)] = SEPARATOR;
	else if (line[(*i)] == '<')
		line[(*i)] = LEFTCHEVRON;
	else if (line[(*i)] == '>')
		line[(*i)] = RIGHTCHEVRON;
}

static int	_detect_missing_quote(int single_quote_open, int double_quote_open)
{
	if (single_quote_open || double_quote_open)
		g_returnval = 2;
	if (single_quote_open)
		return (ft_putstr_fd \
		("minishell: syntax error: ending quote missing (')\n", 2), 0);
	else if (double_quote_open)
		return (ft_putstr_fd \
		("minishell: syntax error: ending quote missing (\")\n", 2), 0);
	else
		return (1);
}

int	quotes_interpretation(t_env *environment, char **line)
{
	char	*ptr;
	size_t	i;
	int		single_quote_open;
	int		double_quote_open;

	ft_strip(*line);
	i = 0;
	single_quote_open = FALSE;
	double_quote_open = FALSE;
	while ((*line)[i])
	{
		if (single_quote_open)
			_actions_singlequoteopen((*line), &single_quote_open, &i);
		else if (double_quote_open)
			_actions_doublequoteopen((*line), &double_quote_open, &i);
		else
			_actions_default((*line), \
			&single_quote_open, &double_quote_open, &i);
		ptr = *line;
		(*line) = replace_key_by_value(environment, (*line));
		if (ptr == *line)
			i++;
	}
	return \
	(_detect_missing_quote(single_quote_open, double_quote_open));
}
