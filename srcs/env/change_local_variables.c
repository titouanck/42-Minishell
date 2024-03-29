/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_local_variables.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 12:06:15 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_parse_key(char **line, char **key)
{
	int		append;
	size_t	i;
	char	*key_start;
	char	*key_end;

	*key = NULL;
	i = 0;
	key_start = (*line) + i;
	while (ft_inset((*line)[i], VARNAMESET))
		i++;
	key_end = (*line) + i;
	if (key_start == key_end)
		return (0);
	if (!((*line)[i] == '=' || ((*line)[i] == '+' && (*line)[i + 1] == '=')))
		return (0);
	append = 0;
	*key = db_strndup(key_start, key_end - key_start);
	if ((*line)[i] == '+' && (*line)[i + 1] == '=')
	{
		*line = key_end + 2;
		append = 1;
	}
	else
		*line = key_end + 1;
	return (append);
}

static void	_select_action(char c, \
	int *single_quote_open, int *double_quote_open)
{
	if (*single_quote_open)
	{
		if (c == '\'')
			*single_quote_open = FALSE;
	}
	else if (*double_quote_open)
	{
		if (c == '\"')
		*double_quote_open = FALSE;
	}
	else
	{
		if (c == '\'')
			*single_quote_open = TRUE;
		else if (c == '\"')
			*double_quote_open = TRUE;
	}
}

static int	_parse_value(t_env *environment, char **line, char **value)
{
	char	*value_start;
	char	*value_end;
	int		single_quote_open;
	int		double_quote_open;

	single_quote_open = FALSE;
	double_quote_open = FALSE;
	value_start = (*line);
	value_end = value_start;
	while (*value_end && (single_quote_open || double_quote_open \
	|| !ft_iswhitespace(*value_end)))
	{
		_select_action(*value_end, &single_quote_open, &double_quote_open);
		value_end++;
	}
	if (value_start == value_end)
		return (1);
	*line = value_end;
	*value = db_strndup(value_start, value_end - value_start);
	if (!(*value))
		exit_erralloc(environment);
	if (!quotes_interpretation(environment, value))
		return (db_free(*value), 0);
	return (1);
}

static char	*_check_str(char **line)
{
	char	*ptr;

	ptr = (*line);
	if (!(*line))
		return (NULL);
	while (ft_iswhitespace(*(*line)))
		(*line)++;
	if (!(*(*line)))
		return (NULL);
	return (ptr);
}

int	change_local_variables(t_env *environment, char *line, size_t size)
{
	char	*ptr;
	char	*key;
	int		append;
	char	*value;

	ptr = _check_str(&line);
	if (!ptr)
		return (0);
	append = _parse_key(&line, &key);
	value = NULL;
	if (!key)
		return (0);
	else if (!_parse_value(environment, &line, &value))
		return (db_free(key), -1);
	if (!value)
		value = db_strdup("");
	if (!value)
		exit_erralloc(environment);
	ft_memmove(ptr, line, ft_strlen(line) + 1);
	if (size > 1)
		return (db_free(key), db_free(value), 1);
	else
		update_local_variables(environment, key, value, append);
	return (1);
}
