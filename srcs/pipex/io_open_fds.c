/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_open_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	io_open_fds(t_redirect *redirect)
{
	if (!redirect)
		return (0);
	if (redirect->infile)
	{
		redirect->fd_infile = open(redirect->infile, O_RDONLY);
		if (redirect->fd_infile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redirect->infile);
			free(redirect->infile);
			redirect->infile = NULL;
			free(redirect->outfile);
			redirect->outfile = NULL;
			return (0);
		}
	}
	if (redirect->outfile)
	{
		if (redirect->append == 0)
			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else
			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (redirect->fd_outfile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redirect->outfile);
			if (redirect->infile)
				close(redirect->fd_infile);
			free(redirect->infile);
			redirect->infile = NULL;
			free(redirect->outfile);
			redirect->outfile = NULL;
			return (0);
		}
	}
	return (1);
}
