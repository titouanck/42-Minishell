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

static int	_first_p(char **sec_p, int *question_mark, char *line, size_t i)
{
	(*question_mark) = FALSE;
	if (line[i + 1] == '\?')
		(*question_mark) = TRUE;
	if ((*question_mark))
		(*sec_p) = line + i + 2;
	else
	{
		(*sec_p) = line + i + 1;
		if (!ft_isdigit(*(*sec_p)))
			while (*(*sec_p) && ft_strinset((*sec_p), VARNAMESET, 1))
				(*sec_p)++;
		else
			(*sec_p)++;
	}
	return (1);
}

static char	*_sec_p_qm(char **first_p, char **sec_p, char *line, size_t i)
{
	char	*var;
	char	*tmp;
	t_env	*environment;

	environment = saved_environment(NULL);
	var = (*sec_p);
	(*sec_p) = db_strdup((*sec_p));
	if (!(*sec_p))
		return (NULL);
	*var = '\0';
	var = line + i + 1;
	var = ft_itoa(g_returnval);
	if (!var)
		var = "";
	tmp = db_strrjoin((*first_p), var, (*sec_p));
	db_free(var);
	db_free(line);
	db_free((*first_p));
	db_free((*sec_p));
	line = tmp;
	return (line);
}

static char	*_sec_p_noqm(char **first_p, char **sec_p, char *line, size_t i)
{
	char	*var;
	char	*tmp;
	t_env	*environment;

	environment = saved_environment(NULL);
	var = (*sec_p);
	(*sec_p) = db_strdup((*sec_p));
	if (!(*sec_p))
		return (NULL);
	*var = '\0';
	var = line + i + 1;
	var = get_value_by_key(environment, var);
	if (!var)
		var = "";
	tmp = db_strrjoin((*first_p), var, (*sec_p));
	db_free(line);
	db_free((*first_p));
	db_free((*sec_p));
	line = tmp;
	return (line);
}

char	*replace_key_by_value(char *line)
{
	char	*first_p;
	char	*sec_p;
	int		question_mark;
	size_t	i;

	i = -1;
	while (line && line[++i])
	{
		if (line[i] == VARKEY)
		{
			line[i] = '\0';
			first_p = db_strdup(line);
			if (!first_p)
				return (db_free(line), NULL);
			_first_p(&sec_p, &question_mark, line, i);
			if (question_mark)
				line = _sec_p_qm(&first_p, &sec_p, line, i);
			else
				line = _sec_p_noqm(&first_p, &sec_p, line, i);
			if (!line)
				return (db_free(line), db_free(first_p), NULL);
			i--;
		}
	}
	return (line);
}
