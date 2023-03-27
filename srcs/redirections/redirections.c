/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/27 15:41:58 by tchevrie         ###   ########.fr       */
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
		return (new_redirection(environment, line + i + (*j), \
		redirect, INFILE_HEREDOC));
	}
	else if (line[i + (*j)])
	{
		return (new_redirection(environment, line + i + (*j), \
		redirect, INFILE_FILE));
	}
	else
		return (redirection_check_syntax(environment, \
		line + i + (*j), redirect));
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
		return (new_redirection(environment, line + i + j, \
		redirect, OUTFILE_APPEND));
	}
	else if (line[i + j])
	{
		return (new_redirection(environment, line + i + j, \
		redirect, OUTFILE_TRUNC));
	}
	else
		return (redirection_check_syntax(environment, \
		line + i + j, redirect));
	return (1);
}

static int	detect_redirections(char *line, \
	t_redirect *redirect)
{
	size_t	i;
	size_t	j;
	int		r;

	i = 0;
	while (line[i])
	{
		j = 0;
		if (line[i + j] == LEFTCHEVRON)
		{
			r = _left_chevron(redirect, line, i, &j);
			if (r != 1)
				return (r);
		}
		else if (line[i + j] == RIGHTCHEVRON)
		{
			r = _right_chevron(redirect, line, i, j);
			if (r != 1)
				return (r);
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
