/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_local_variables.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/27 14:31:48 by tchevrie         ###   ########.fr       */
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
	*key = ft_strdupn(key_start, key_end - key_start);
	if (!(*key))
		ft_putstr_fd(ERRALLOC, 2);
	if ((*line)[i] == '+' && (*line)[i + 1] == '=')
	{
		*line = key_end + 2;
		append = 1;
	}
	else
		*line = key_end + 1;
	return (append);
}

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

static void	_actions_default(char c, \
	int *single_quote_open, int *double_quote_open)
{
	if (c == '\'')
		*single_quote_open = TRUE;
	else if (c == '\"')
		*double_quote_open = TRUE;
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
	while (*value_end && (single_quote_open || double_quote_open || !ft_iswhitespace(*value_end)))
	{
		if (single_quote_open)
			_actions_singlequoteopen(*value_end, &single_quote_open);
		else if (double_quote_open)
			_actions_doublequoteopen(*value_end, &double_quote_open);
		else
			_actions_default(*value_end, \
			&single_quote_open, &double_quote_open);
		value_end++;
	}
	if (value_start == value_end)
		return (1);
	*line = value_end;
	*value = ft_strdupn(value_start, value_end - value_start);
	if (!(*value))
		ft_putstr_fd(ERRALLOC, 2);
	if (!quotes_interpretation(environment, value))
		return (free(*value), 0);
	return (1);
}


static void	_export_element(t_env *environment, \
	char *key, char *value, int append)
{
	t_env	*elem;
	char	*tmp;
	
	elem = environment;
	if (!key || !elem)
		return ;
	elem = elem->next;
	while (elem)
	{
		if (ft_strcmp(elem->key, key) == 0)
		{
			free(key);
			tmp = elem->value;
			if (value && append)
			{
				elem->value = ft_strjoin(elem->value, value);
				free(value);
			}
			else if (!append)
				elem->value = value;
			free(tmp);
			return ;
		}
		elem = elem->next;
	}
	env_lstaddback(environment, key, value, 0);
}

int	change_local_variables(t_env *environment, char *line, size_t size)
{
	char	*ptr;
	char	*key;
	int		append;
	char	*value;

	ptr = line;
	if (!line)
		return (0);
	while (ft_iswhitespace(*line))
		line++;
	if (!(*line))
		return (0);
	append = _parse_key(&line, &key);
	value = NULL;
	if (!key)
		return (0);
	else
		if(!_parse_value(environment, &line, &value))	
			return (free(key), -1);
	if (!value)
		value = ft_strdup("");
	if (!value)
		return (ft_putstr_fd(ERRALLOC, 2), free(key), -1);
	ft_memmove(ptr, line, ft_strlen(line) + 1);
	if (size > 1)
	{
		free(key);
		free(value);
	}
	else
		_export_element(environment, key, value, append);
	return (1);
}
