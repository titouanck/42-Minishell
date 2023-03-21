/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_open_fds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:33:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 18:51:41 by tchevrie         ###   ########.fr       */
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
	if (size == 2 && ft_strcmp(tab[0], "dev") == 0 && ft_strcmp(tab[1], "stdout") == 0)
		r = 1;
	else
		r = 0;
	db_freetab(tab);
	return (r);
}

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

int	io_open_fds(t_env *environment, t_redirect *redirect)
{
	t_redirectionlst	*current;
	int					heredoc;

	environment->log.infile = NULL;
	environment->log.outfile = NULL;
	if (!redirect || !redirect->lst)
		return (0);
	redirect->fd_infile = -1;
	redirect->fd_outfile = -1;
	if (redirect->infile)
		heredoc = 1;
	else
		heredoc = 0;
	current = redirect->lst->next;
	while (current)
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
			{
				if (redirect->fd_outfile != -1)
					close(redirect->fd_outfile);
				if (redirect->outfile)
					db_free(redirect->outfile);
				redirect->outfile = db_strdup(current->str);
				if (current->redirection_type == 'w')
					redirect->fd_outfile = open(current->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
				else if (current->redirection_type == 'a')
					redirect->fd_outfile = open(current->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
				if (redirect->fd_outfile == -1)
					return (error_on_open(redirect, current->str), 0);
			}
		}
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
	// if (redirect->infile)
	// {
	// 	redirect->fd_infile = open(redirect->infile, O_RDONLY);
	// 	if (redirect->fd_infile == -1)
	// 	{
	// 		if (ft_strlen(redirect->infile) > 0)
	// 			ft_putstr_fd("minishell: ", 2);
	// 		else
	// 			ft_putstr_fd("minishell: : ", 2);
	// 		perror(redirect->infile);
	// 		db_free(redirect->infile);
	// 		redirect->infile = NULL;
	// 		db_free(redirect->outfile);
	// 		redirect->outfile = NULL;
	// 		g_returnval = 1;
	// 		return (0);
	// 	}
	// }
	// if (redirect->outfile)
	// {
	// 	if (!check_for_stdout(redirect->outfile))
	// 	{
	// 		if (redirect->append == 0)
	// 			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	// 		else
	// 			redirect->fd_outfile = open(redirect->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	// 		if (redirect->fd_outfile == -1)
	// 		{
	// 			if (ft_strlen(redirect->outfile) > 0)
	// 				ft_putstr_fd("minishell: ", 2);
	// 			else
	// 				ft_putstr_fd("minishell: : ", 2);
	// 			perror(redirect->outfile);
	// 			if (redirect->infile)
	// 				close(redirect->fd_infile);
	// 			db_free(redirect->infile);
	// 			redirect->infile = NULL;
	// 			db_free(redirect->outfile);
	// 			redirect->outfile = NULL;
	// 			g_returnval = 1;
	// 			return (0);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		db_free(redirect->outfile);
	// 		redirect->outfile = NULL;
	// 	}
	// }
	return (1);
}
