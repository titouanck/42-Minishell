/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 12:48:16 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:29:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	_heredoc_limiter_between_quotes(char *line)
{
	size_t	i;

	if (line[0] == '<' && line[1] == '<')
	{
		environment->limiter_between_quotes = 0;
		i = 2;
		while (ft_iswhitespace(line[i]))
			i++;
		if (line[i] == '\'' || line[i] == '\"')
		{
			environment->limiter_between_quotes = 1;
		}
		if (line[i] == '\'')
		{
			while (line[++i] && line[i] != '\'')
			{
				if (line[i] == '$')
					line[i] = NOTAVARKEY;
			}
		}	
		else if (line[i] == '\"')
		{
			while (line[++i] && line[i] != '\"')
			{
				if (line[i] == '$')
					line[i] = NOTAVARKEY;
			}
		}
		else
		{
			while (line[i] && line[i] != '\"' && line[i] != '\'')
			{
				if (line[i] == '$')
					line[i] = NOTAVARKEY;
				i++;
			}
		}	
	}
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
		environment->g_returnval = 2;
	if (single_quote_open)
		return (ft_putstr_fd \
		("minishell: syntax error: ending quote missing (')\n", 2), 0);
	else if (double_quote_open)
		return (ft_putstr_fd \
		("minishell: syntax error: ending quote missing (\")\n", 2), 0);
	else
		return (1);
}

static int _detect_empty_redirections(char *line)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (line[i])
	{
		if (line[i] == '>' || (line[i] == '<' && (i == 0 || line[i - 1] != '<')))
		{
			j = i + 1;
			while (ft_iswhitespace(line[j]))
				j++;
			if  ((line[j] == '\"' && line[j + 1] == '\"') || (line[j] == '\'' && line[j + 1] == '\''))
			{
				while ((line[j] == '\"' && line[j + 1] == '\"') || (line[j] == '\'' && line[j + 1] == '\''))
					j += 2;
				if (!line[j] || ft_iswhitespace(line[j]))
					return (ft_putstr_fd("minishell: : No such file or directory\n", 2), 0);
			}
		}
		i++;
	}
	return (1);
}

int	quotes_interpretation(char **line)
{
	char	*ptr;
	size_t	i;
	int		single_quote_open;
	int		double_quote_open;

	ft_strip(*line);
	i = 0;
	single_quote_open = FALSE;
	double_quote_open = FALSE;
	if (_detect_empty_redirections(*line) == 0)
		return (0);
	environment->limiter_between_quotes = 0;
	while ((*line)[i])
	{
		if (single_quote_open)
			_actions_singlequoteopen((*line), &single_quote_open, &i);
		else if (double_quote_open)
			_actions_doublequoteopen((*line), &double_quote_open, &i);
		else
		{
			_heredoc_limiter_between_quotes(*line + i);
			_actions_default((*line), \
			&single_quote_open, &double_quote_open, &i);
		}
		ptr = *line;
		(*line) = replace_key_by_value((*line));
		if (ptr == *line)
			i++;
	}
	i = -1;
	while ((*line)[++i])
		if ((*line)[i] == NOTAVARKEY)
			(*line)[i] = '$';
	return \
	(_detect_missing_quote(single_quote_open, double_quote_open));
}
