/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/15 14:43:10 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredocs(t_heredoc *heredoc)
{
	t_heredoc *tmp;

	while (heredoc)
	{
		tmp = heredoc->next;
		db_free(heredoc->limiter);
		db_free(heredoc);
		heredoc = tmp;
	}
}

static void	_remove_quote_token_line(char *line)
{
	size_t	i;

	if (!line)
		return ;
	i = 0;
	while (line[i])
	{
		if (line[i] == QUOTES)
		{
			ft_memmove(line + i, line + i + 1, ft_strlen(line + i + 1) + 1);
			i--;
		}
		i++;
	}
}

void	ft_free_redirect(t_redirect *redirect)
{
	int	fd;

	if (!redirect)
		return ;
	if (redirect->heredoc)
		free_heredocs(redirect->heredoc);
	db_free(redirect->infile);
	db_free(redirect->outfile);
	db_free(redirect);
}

t_heredoc *lstnew_heredoc(t_heredoc *heredoc, char *limiter)
{
	t_heredoc	*new;
	t_heredoc	*current;

	new = db_malloc(sizeof(t_heredoc));
	if (!new)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	new->limiter = limiter;
	new->next = NULL;
	if (!heredoc)
		return (new);
	current = heredoc;
	while (current->next)
		current = current->next;
	current->next = new;
	return (heredoc);
}

static int _leftchevron(t_env *environment, char *line, t_redirect *redirect)
{
	size_t	old_i;
	size_t	i;
	char	*start;
	char	*end;
	char	*limiter;
	int		fd;

	i = 0;
	while (line[i])
	{
		old_i = i;
		if (line[i] == LEFTCHEVRON)
		{
			line[i] = SEPARATOR;
			i++;
			if (line[i] == LEFTCHEVRON)
			{
				line[i] = HEREDOC;
				i++;
				while (line[i] == SEPARATOR)
					i++;
				if (!line[i] || line[i] == LEFTCHEVRON || line[i] == RIGHTCHEVRON)
					return (ft_putstr_fd("minishell: syntax error: expected limiter near `<<\'\n", 2), ft_syntaxerror(environment, NULL), -1);
				start = line + i;
				while (line[i] > 0 || line[i] == QUOTES)
					i++;
				end = line + i;
				if (redirect->infile)
				{
					fd = open(redirect->infile, O_RDONLY);
					if (fd == -1)
					{
						ft_putstr_fd("minishell: ", 2);
						perror(redirect->infile);
						return (0);
					}
					else
						close(fd);
					db_free(redirect->infile);
				}
				limiter = db_strndup(start, end - start);
				if (!limiter)
					return (0);
				_remove_quote_token_line(limiter);
				redirect->heredoc = lstnew_heredoc(redirect->heredoc, limiter);
				redirect->infile = NULL;
				ft_memmove(line + old_i, line + i, ft_strlen(line + i) + 1);
			}
			else
			{
				while (line[i] == SEPARATOR)
					i++;
				if (!line[i] || line[i] == LEFTCHEVRON || line[i] == RIGHTCHEVRON)
					return (ft_putstr_fd("minishell: syntax error: expected infile near `<\'\n", 2), ft_syntaxerror(environment, NULL), -1);
				start = line + i;
				while (line[i] > 0 || line[i] == QUOTES)
					i++;
				end = line + i;
				if (redirect->infile)
				{
					fd = open(redirect->infile, O_RDONLY);
					if (fd == -1)
					{
						ft_putstr_fd("minishell: ", 2);
						perror(redirect->infile);
						return (0);
					}
					else
						close(fd);
					db_free(redirect->infile);
				}
				redirect->infile = db_strndup(start, end - start);
				_remove_quote_token_line(redirect->infile);
				ft_memmove(line + old_i, line + i, ft_strlen(line + i) + 1);
			}
			i = old_i - 1;
		}
		i++;
	}
	return (1);
}

static int _rightchevron(t_env *environment, char *line, t_redirect *redirect)
{
	size_t	old_i;
	size_t	i;
	char	*start;
	char	*end;
	int		fd;
	int		old_append;

	i = 0;
	while (line[i])
	{
		old_i = i;
		old_append = redirect->append;
		if (line[i] == RIGHTCHEVRON)
		{
			line[i] = SEPARATOR;
			i++;
			if (line[i] == RIGHTCHEVRON)
			{
				line[i] = SEPARATOR;
				i++;
				redirect->append = 1;
			}
			else
				redirect->append = 0;
			while (line[i] == SEPARATOR)
				i++;
			if (!line[i] || line[i] == LEFTCHEVRON || line[i] == RIGHTCHEVRON)
				return (ft_putstr_fd("minishell: syntax error: expected outfile near `>\'\n", 2), ft_syntaxerror(environment, NULL), -1);
			start = line + i;
			while (line[i] > 0 || line[i] == QUOTES)
				i++;
			end = line + i;
			if (redirect->outfile)
			{
				if (old_append == 0)
					fd = open(redirect->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
				else
					fd = open(redirect->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
				if (fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(redirect->outfile);
					return (0);
				}
				else
					close(fd);
				db_free(redirect->outfile);
			}
			redirect->outfile = db_strndup(start, end - start);
			_remove_quote_token_line(redirect->outfile);
			ft_memmove(line + old_i, line + i, ft_strlen(line + i) + 1);
			i = old_i - 1;
		}
		i++;
	}
	return (1);
}

int	heredoc_file(t_env *environment, t_redirect *redirect)
{
	int				fd;
	char			*filename;
	char			*tmp;
	size_t			i;

	filename = ft_randomstr("/tmp/minishell-heredoc_", NULL, 16);
	if (!filename)
		return (ft_putstr_fd(ERRALLOC, 2), -1);
	tmp = environment->heredoc_files;
	environment->heredoc_files = db_strrjoin(environment->heredoc_files, "|", filename);
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

int	use_heredoc(t_env *environment, t_redirect *redirect)
{
	t_heredoc	*current;
	char		*line;
	int			fd;
	size_t		i;
	int			returnval;

	if (!redirect || !redirect->heredoc)
		return (1);
	fd = -1;
	if (redirect->infile == NULL && redirect->heredoc)
		fd = heredoc_file(environment, redirect);
	current = redirect->heredoc;
	returnval = g_returnval;
	g_returnval = 0;
	while (current)
	{
		heredoc_signal_behavior();
		while (1)
		{
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
			{
				db_free(line);
				break ;
			}
			if (!line)
			{
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(current->limiter, 2);
				ft_putstr_fd("\')\n", 2);
				break ;
			}
			if (ft_strcmp(current->limiter, line) == 0)
			{
				db_free(line);
				break ;
			}
			if (!(environment->limiter_between_quotes))
			{
				i = -1;
				while (line[++i])
					if (line[i] == '$')
						line[i] = VARKEY;
				line = replace_key_by_value(environment, line);
			}
			if (fd != -1 && current->next == NULL)
			{
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 1);
			}
			db_free(line);
		}
		if (g_returnval == 130)
			break ;
		current = current->next;
	}
	if (use_readline())
		default_signal_behavior();
	else
		notatty_signal_behavior();
	ft_swap(&returnval, &g_returnval);
	if (returnval == 130)
		g_returnval = returnval;
	if (fd != -1)
		close(fd);
	free_heredocs(redirect->heredoc);
	redirect->heredoc = NULL;
	if (returnval == 130)
		return (0);
	return (1);
}

t_redirect	*redirections(t_env *environment, char *line, int empty)
{
	t_redirect	*redirect;
	int			leftreturn;
	int			rightreturn;

	redirect = db_malloc(sizeof(t_redirect));
	if (!redirect)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	redirect->infile = NULL;
	redirect->heredoc = NULL;
	redirect->outfile = NULL;
	redirect->append = 0;
	if (!empty)
	{
		redirect->to_execute = TRUE;
		leftreturn = _leftchevron(environment, line, redirect);
		if (leftreturn == -1)
		{
			g_returnval = 2;
			return (ft_free_redirect(redirect), NULL);
		}
		else if (leftreturn == 0)
		{
			redirect->to_execute = FALSE;
			free_heredocs(redirect->heredoc);
			redirect->heredoc = NULL;
		}
		rightreturn = _rightchevron(environment, line, redirect);
		if (rightreturn == -1)
		{
			g_returnval = 2;
			return (ft_free_redirect(redirect), NULL);
		}
		else if (leftreturn == 0)
			redirect->to_execute = FALSE;
		if (!use_heredoc(environment, redirect))
		{
			ft_free_redirect(redirect);
			return (NULL);
		}
	}
	else
	{
		redirect->to_execute = FALSE;
		line[0] = '\0';
	}
	return (redirect);
}
