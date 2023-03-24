/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 12:48:16 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 19:58:48 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_actions_quote_open(char *line, \
	int *single_open, int *double_open, size_t *i)
{
	if (*single_open)
	{
		if (line[(*i)] == '\'')
		{
			*single_open = FALSE;
			ft_memmove(line + (*i), line + (*i) + 1, ft_strlen(line + (*i)));
			(*i)--;
		}
	}
	else if (*double_open)
	{
		if (line[(*i)] == '\"')
		{
			*double_open = FALSE;
			ft_memmove(line + (*i), line + (*i) + 1, ft_strlen(line + (*i)));
			(*i)--;
		}
		else if (line[(*i)] == '$' && line[(*i) + 1] \
		&& ft_strinset(line + (*i) + 1, VARNAMESET "?", 1))
			line[(*i)] = VARKEY;
	}
}

static void	_actions_default(char *line, \
	int *single_open, int *double_open, size_t *i)
{
	parse_heredoc_limiter(saved_environment(NULL), line + *i);
	if (line[(*i)] == '\'')
	{
		*single_open = TRUE;
		line[*i] = QUOTES;
	}
	else if (line[(*i)] == '\"')
	{
		*double_open = TRUE;
		line[*i] = QUOTES;
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

static int	_detect_missing_quote(t_env *environment, \
	int single_open, int double_open)
{
	if (single_open || double_open)
		g_returnval = 2;
	if (single_open)
		return (ft_putstr_fd \
		("minishell: syntax error: ending quote missing (')\n", 2), \
		ft_syntaxerror(environment, NULL), 0);
	else if (double_open)
		return (ft_putstr_fd \
		("minishell: syntax error: ending quote missing (\")\n", 2), \
		ft_syntaxerror(environment, NULL), 0);
	else
		return (1);
}

static void	_format_line_and_init(t_env *env, char **line, \
	int *single_open, int *double_open)
{
	ft_strip(*line);
	*single_open = FALSE;
	*double_open = FALSE;
	if (env)
		env->limiter_between_quotes = 0;
}

int	quotes_interpretation(t_env *env, char **line)
{
	char	*ptr;
	size_t	i;
	int		single_open;
	int		double_open;

	_format_line_and_init(env, line, &single_open, &double_open);
	i = 0;
	while ((*line)[i])
	{
		if (single_open || double_open)
			_actions_quote_open((*line), &single_open, &double_open, &i);
		else
			_actions_default((*line), &single_open, &double_open, &i);
		ptr = *line;
		(*line) = replace_key_by_value((*line));
		if (ptr == *line)
			i++;
	}
	i = -1;
	while ((*line)[++i])
		if ((*line)[i] == NOTAVARKEY)
			(*line)[i] = '$';
	return (_detect_missing_quote(env, single_open, double_open));
}
