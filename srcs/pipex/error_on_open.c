/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_on_open.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:35:46 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 15:36:03 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_on_open(t_redirect *redirect, char *file)
{
	if (ft_strlen(file) > 0)
		ft_putstr_fd("minishell: ", 2);
	else
		ft_putstr_fd("minishell: : ", 2);
	perror(file);
	if (redirect->fd_infile != -1)
		close(redirect->fd_infile);
	redirect->fd_infile = -1;
	if (redirect->fd_outfile != -1)
		close(redirect->fd_outfile);
	redirect->fd_outfile = -1;
	db_free(redirect->infile);
	redirect->infile = NULL;
	db_free(redirect->outfile);
	redirect->outfile = NULL;
	redirection_lstclear(redirect->lst);
	redirect->lst = NULL;
	g_returnval = 1;
}
