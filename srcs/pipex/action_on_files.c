/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_on_files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:33:54 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 17:30:53 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_for_stdout(char *output)
{
	char	**tab;
	size_t	size;
	int		r;

	if (!output || output[0] != '/')
		return (0);
	tab = db_split(output, '/');
	if (!tab)
		return (0);
	size = 0;
	while (tab[size])
		size++;
	if (size == 2 && ft_strcmp(tab[0], "dev") == 0 \
	&& ft_strcmp(tab[1], "stdout") == 0)
		r = 1;
	else
		r = 0;
	db_freetab(tab);
	return (r);
}

static int	_action_stdout_detected(t_redirect *redirect, \
	t_redirlst *current)
{
	if (redirect->fd_outfile != -1)
		close(redirect->fd_outfile);
	if (redirect->outfile)
		db_free(redirect->outfile);
	redirect->outfile = db_strdup(current->str);
	if (current->redirection_type == 'w')
		redirect->fd_outfile = \
		open(current->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (current->redirection_type == 'a')
		redirect->fd_outfile = \
		open(current->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (redirect->fd_outfile == -1)
		return (error_on_open(redirect, current->str), 0);
	return (1);
}

int	action_on_files(t_redirect *redirect, \
	t_redirlst *current, int heredoc)
{
	if (current->str)
	{
		if (current->redirection_type == 'r')
		{
			if (redirect->fd_infile != -1)
				close(redirect->fd_infile);
			if (!heredoc)
			{
				if (redirect->infile)
					db_free(redirect->infile);
				redirect->infile = db_strdup(current->str);
			}
			redirect->fd_infile = open(current->str, O_RDONLY);
			if (redirect->fd_infile == -1)
				return (error_on_open(redirect, current->str), 0);
		}
		else if (!check_for_stdout(current->str))
			return (_action_stdout_detected(redirect, current));
	}
	return (1);
}
