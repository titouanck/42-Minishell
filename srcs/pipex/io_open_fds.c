/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_open_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 15:34:06 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_init(t_env *environment, t_redirect *redirect, int *heredoc)
{
	environment->log.infile = NULL;
	environment->log.outfile = NULL;
	if (!redirect || !redirect->lst)
		return (0);
	redirect->fd_infile = -1;
	redirect->fd_outfile = -1;
	if (redirect->infile)
		*heredoc = 1;
	else
		*heredoc = 0;
	return (1);
}

int	io_open_fds(t_env *environment, t_redirect *redirect)
{
	t_redirectionlst	*current;
	int					heredoc;

	if (!_init(environment, redirect, &heredoc))
		return (0);
	current = redirect->lst->next;
	while (current)
	{
		if (!action_on_files(redirect, current, heredoc))
			return (0);
		current = current->next;
	}
	if (heredoc)
	{
		if (redirect->fd_infile != -1)
			close(redirect->fd_infile);
		redirect->fd_infile = open(redirect->infile, O_RDONLY);
		if (redirect->fd_infile == -1)
			return (error_on_open(redirect, redirect->infile), 0);
	}
	redirection_lstclear(redirect->lst);
	redirect->lst = NULL;
	environment->log.infile = db_strdup(redirect->infile);
	environment->log.outfile = db_strdup(redirect->outfile);
	return (1);
}
