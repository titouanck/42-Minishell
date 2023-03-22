/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:08:42 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 17:46:25 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_file(t_env *environment, t_redirect *redirect)
{
	int				fd;
	char			*filename;
	char			*tmp;

	filename = ft_randomstr("/tmp/minishell-heredoc_", NULL, 16);
	if (!filename)
		exit_erralloc(environment);
	tmp = environment->heredoc_files;
	environment->heredoc_files = \
	db_strrjoin(environment->heredoc_files, "|", filename);
	db_free(tmp);
	dynamic_memory_address_db('+', filename);
	redirect->infile = filename;
	fd = open(redirect->infile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		db_free(filename);
		redirect->infile = NULL;
	}
	return (fd);
}

static char	*_read_next_line(t_redirlst *current)
{
	char		*line;

	if (use_readline())
	{
		line = readline("> ");
		dynamic_memory_address_db('+', line);
	}
	else
	{
		line = get_next_line(0);
		if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	if (g_returnval == 130)
		return (db_free(line), NULL);
	if (!line)
	{
		ft_putstr_fd("minishell: warning: " \
		"here-document delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(current->str, 2);
		ft_putstr_fd("\')\n", 2);
		return (NULL);
	}
	return (line);
}

static int	_actions_on_line(t_env *environment, char *line, \
	t_redirlst *current, int fd)
{
	size_t	i;

	if (ft_strcmp(current->str, line) == 0)
	{
		db_free(line);
		return (0);
	}
	if (!(environment->limiter_between_quotes))
	{
		i = -1;
		while (line[++i])
			if (line[i] == '$')
				line[i] = VARKEY;
		line = replace_key_by_value(line);
	}
	if (fd != -1)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	db_free(line);
	return (1);
}

static void	_open_them_all(t_env *environment, t_redirect *redirect, \
	t_redirlst *current)
{
	char		*line;
	int			fd;

	fd = -1;
	while (current)
	{
		if (current->redirection_type == INFILE_HEREDOC)
		{
			if (current->next == NULL)
				fd = heredoc_file(environment, redirect);
			heredoc_signal();
			while (1)
			{
				line = _read_next_line(current);
				if (!line || !_actions_on_line(environment, line, current, fd))
					break ;
			}
			if (g_returnval == 130)
				break ;
		}
		current = current->next;
	}
	if (fd != -1)
		close(fd);
}

int	open_heredoc(t_env *environment, t_redirect *redirect)
{
	t_redirlst	*current;
	int			returnval;

	returnval = g_returnval;
	g_returnval = 0;
	current = redirect->lst;
	_open_them_all(environment, redirect, current);
	current = redirect->lst;
	while (current)
	{
		if (current->redirection_type == INFILE_HEREDOC)
			current = redirection_lstdel(redirect->lst, current->str);
		else
			current = current->next;
	}
	if (use_readline())
		default_signal_tty();
	else
		default_signal_notty();
	ft_swap(&returnval, &g_returnval);
	if (returnval == 130)
		g_returnval = returnval;
	if (returnval == 130)
		return (0);
	return (1);
}
