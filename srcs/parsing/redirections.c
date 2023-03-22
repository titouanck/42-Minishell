/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 12:58:28 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	_remove_quote_token_line(char *line)
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
	if (!redirect)
		return ;
	if (redirect->lst)
		redirection_lstclear(redirect->lst);
	db_free(redirect->infile);
	db_free(redirect->outfile);
	db_free(redirect);
}

#define INFILE_FILE 'r'
#define INFILE_HEREDOC 'h'
#define OUTFILE_TRUNC 'w'
#define OUTFILE_APPEND 'a'

static int	check_syntax_error(t_env *environment, char c, t_redirect *redirect)
{
	if (c == LEFTCHEVRON)
		ft_syntaxerror(environment, "<");
	else if (c == RIGHTCHEVRON)
		ft_syntaxerror(environment, ">");
	else if (!c && redirect->last)
		ft_syntaxerror(environment, "newline");
	else if (!c && !redirect->last)
		ft_syntaxerror(environment, "|");
	else
		return (1);
	redirection_lstclear(redirect->lst);
	redirect->lst = NULL;
	return (0);
}

static int	new_redirection(t_env *environment, char *line, t_redirect *redirect, int redirection_type)
{
	size_t	i;
	char	*start;
	char	*end;
	char	*new;
	
	i = 0;
	while (line[i] == SEPARATOR)
		i++;
	if (!check_syntax_error(environment, line[i], redirect))
		return (0);
	start = line + i;
	while (line[i] > 0 || line[i] == QUOTES)
		i++;
	end = line + i;
	new = db_strndup(start, end - start);
	_remove_quote_token_line(new);
	ft_memmove(line, line + i, ft_strlen(line + i) + 1);
	redirect->lst = redirection_lstaddback(redirect->lst, new, redirection_type);
	return (1);
}

static int detect_redirections(t_env *environment, char *line, t_redirect *redirect)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (line[i])
	{
		j = 0;
		if (line[i + j] == LEFTCHEVRON)
		{
			line[i + j] = SEPARATOR;
			j++;
			if (line[i + j] == LEFTCHEVRON)
			{
				line[i + j] = SEPARATOR;
				j++;
				if (!new_redirection(environment, line + i + j, redirect, INFILE_HEREDOC))
					return (0);
			}
			else if (line[i + j])
			{
				if (!new_redirection(environment, line + i + j, redirect, INFILE_FILE))
					return (0);
			}
			else if (!check_syntax_error(environment, line[i + j], redirect))
				return (0);
		}
		else if (line[i + j] == RIGHTCHEVRON)
		{
			line[i + j] = SEPARATOR;
			j++;
			if (line[i + j] == RIGHTCHEVRON)
			{
				line[i + j] = SEPARATOR;
				j++;
				if (!new_redirection(environment, line + i + j, redirect, OUTFILE_APPEND))
					return (0);
			}
			else if (line[i + j])
			{
				if (!new_redirection(environment, line + i + j, redirect, OUTFILE_TRUNC))
					return (0);
			}
			else if (!check_syntax_error(environment, line[i + j], redirect))
				return (0);
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

	filename = ft_randomstr("/tmp/minishell-heredoc_", NULL, 16);
	if (!filename)
		exit_erralloc(environment);
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


int	open_heredoc(t_env *environment, t_redirect *redirect)
{
	t_redirectionlst	*current;
	char		*line;
	int			fd;
	size_t		i;
	int			returnval;

	current = redirect->lst;
	returnval = g_returnval;
	g_returnval = 0;
	fd = -1;
	while (current)
	{
		if (current->redirection_type == INFILE_HEREDOC)
		{
			if (current->next == NULL)
				fd = heredoc_file(environment, redirect);
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
					ft_putstr_fd(current->str, 2);
					ft_putstr_fd("\')\n", 2);
					break ;
				}
				if (ft_strcmp(current->str, line) == 0)
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
					line = replace_key_by_value(line);
				}
				if (fd != -1)
				{
					write(fd, line, ft_strlen(line));
					write(fd, "\n", 1);
				}
				db_free(line);
			}
			if (g_returnval == 130)
				break ;
		}
		current = current->next;
	}
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
	if (fd != -1)
		close(fd);
	if (returnval == 130)
		return (0);
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
	if (!detect_redirections(environment, line, redirect))
		return (ft_free_redirect(redirect), NULL);
	if (!open_heredoc(environment, redirect))
		return (ft_free_redirect(redirect), NULL);
	return (redirect);
}
