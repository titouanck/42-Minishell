/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 14:24:38 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_left_chevron(t_redirect *redirect, char *line, size_t i, size_t *j)
{
	t_env	*environment;

	environment = saved_environment(NULL);
	line[i + (*j)] = SEPARATOR;
	(*j)++;
	if (line[i + (*j)] == LEFTCHEVRON)
	{
		line[i + (*j)] = SEPARATOR;
		(*j)++;
		if (!new_redirection(environment, line + i + (*j), \
		redirect, INFILE_HEREDOC))
			return (0);
	}
	else if (line[i + (*j)])
	{
		if (!new_redirection(environment, line + i + (*j), \
		redirect, INFILE_FILE))
			return (0);
	}
	else if (!redirection_check_syntax(environment, line[i + (*j)], redirect))
		return (0);
	return (1);
}

static int	_right_chevron(t_redirect *redirect, char *line, size_t i, size_t j)
{
	t_env	*environment;

	environment = saved_environment(NULL);
	line[i + j] = SEPARATOR;
	j++;
	if (line[i + j] == RIGHTCHEVRON)
	{
		line[i + j] = SEPARATOR;
		j++;
		if (!new_redirection(environment, line + i + j, \
		redirect, OUTFILE_APPEND))
			return (0);
	}
	else if (line[i + j])
	{
		if (!new_redirection(environment, line + i + j, \
		redirect, OUTFILE_TRUNC))
			return (0);
	}
	else if (!redirection_check_syntax(environment, line[i + j], redirect))
		return (0);
	return (1);
}

static int	detect_redirections(char *line, \
	t_redirect *redirect)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (line[i])
	{
		j = 0;
		if (line[i + j] == LEFTCHEVRON)
		{
			if (!_left_chevron(redirect, line, i, &j))
				return (0);
		}
		else if (line[i + j] == RIGHTCHEVRON)
		{
			if (!_right_chevron(redirect, line, i, j))
				return (0);
		}
		i++;
	}
	return (1);
}

t_redirect	*redirections(t_env *environment, char *line, int last)
{
	t_redirect	*redirect;

	redirect = db_malloc(sizeof(t_redirect));
	if (!redirect)
		exit_erralloc(environment);
	redirect->infile = NULL;
	redirect->outfile = NULL;
	redirect->append = 0;
	redirect->last = last;
	redirect->lst = redirection_lstaddback(NULL, NULL, 0);
	if (!redirect->lst)
		return (ft_free_redirect(redirect), NULL);
	redirect->to_execute = TRUE;
	if (!detect_redirections(line, redirect))
		return (ft_free_redirect(redirect), NULL);
	if (!open_heredoc(environment, redirect))
		return (ft_free_redirect(redirect), NULL);
	return (redirect);
}
