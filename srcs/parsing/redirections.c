/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:23:55 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredocs(t_heredoc *heredoc)
{
	t_heredoc *tmp;

	while (heredoc)
	{
		tmp = heredoc->next;
		free(heredoc->limiter);
		free(heredoc);
		heredoc = tmp;
	}
}

void	free_redirect(t_redirect *redirect)
{
	int	fd;

	if (!redirect)
		return ;
	if (redirect->heredoc)
		free_heredocs(redirect->heredoc);
	if (redirect->heredocfile)
	{
		fd = open(redirect->heredocfile, O_WRONLY | O_TRUNC);
		if (fd != -1)
			close(fd);
	}
	free(redirect->infile);
	free(redirect->outfile);
	free(redirect);
}

t_heredoc *lstnew_heredoc(t_heredoc *heredoc, char *limiter)
{
	t_heredoc	*new;
	t_heredoc	*current;

	new = malloc(sizeof(t_heredoc));
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

static int _leftchevron(char *line, t_redirect *redirect)
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
					return (ft_putstr_fd("minishell: syntax error: expected limiter near `<<\'\n", 2), -1);
				start = line + i;
				while (line[i] > 0)
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
					free(redirect->infile);
				}
				limiter = ft_strndup(start, end - start);
				if (!limiter)
					return (0);
				redirect->heredoc = lstnew_heredoc(redirect->heredoc, limiter);
				redirect->infile = NULL;
				ft_memmove(line + old_i, line + i, ft_strlen(line + i) + 1);
			}
			else
			{
				while (line[i] == SEPARATOR)
					i++;
				if (!line[i] || line[i] == LEFTCHEVRON || line[i] == RIGHTCHEVRON)
					return (ft_putstr_fd("minishell: syntax error: expected infile near `<\'\n", 2), -1);
				start = line + i;
				while (line[i] > 0)
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
					free(redirect->infile);
				}
				redirect->infile = ft_strndup(start, end - start);
				ft_memmove(line + old_i, line + i, ft_strlen(line + i) + 1);
			}
			i = old_i - 1;
		}
		i++;
	}
	return (1);
}

static int _rightchevron(char *line, t_redirect *redirect)
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
				return (ft_putstr_fd("minishell: syntax error: expected outfile near `>\'\n", 2), -1);
			start = line + i;
			while (line[i] > 0)
				i++;
			if (line[i] == EMPTYQUOTE)
			{
				printf("situation\n");
				ft_memmove(line + i, line + i + 1, ft_strlen(line + i) + 1);
				i--;
			}
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
				free(redirect->outfile);
			}
			redirect->outfile = ft_strndup(start, end - start);
			ft_memmove(line + old_i, line + i, ft_strlen(line + i) + 1);
			i = old_i - 1;
		}
		i++;
	}
	return (1);
}

int	heredoc_file(t_redirect *redirect)
{
	int				fd;
	unsigned char	buf[16];
	char			*filename;
	size_t			i;

	filename = ft_strdup("/tmp/.tmpheredocfile_____.minishell");
	if (!filename)
		return (ft_putstr_fd(ERRALLOC, 2), -1);
	redirect->infile = filename;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd != -1 && read(fd, buf, sizeof(buf)) == sizeof(buf))
	{
		close(fd);
		i = -1;
		while (++i < 16)
		{
			while (!ft_isalpha(buf[i]) && !ft_isdigit(buf[i]))
				buf[i] += 21;
			filename[i + 9] = buf[i];
		}
	}
	fd = open(redirect->infile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		free(filename);
		redirect->infile = NULL;
	}
	else
		redirect->heredocfile = filename;
	return (fd);
}

void	use_heredoc(t_redirect *redirect, t_free to_free)
{
	t_heredoc	*current;
	char		*line;
	int			fd;
	size_t		i;
	pid_t		pid;

	if (!redirect || !redirect->heredoc)
		return ;
	fd = -1;
	if (redirect->infile == NULL && redirect->heredoc)
		fd = heredoc_file(redirect);
	pid = fork();
	if (pid == 0)
	{
		heredoc_signal_behavior();
		current = redirect->heredoc;
		while (current)
		{
			while (1)
			{
				line = readline("> ");
				if (!line)
				{
					ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
					ft_putstr_fd(current->limiter, 2);
					ft_putstr_fd("\')\n", 2);
					break ;
				}
				if (ft_strcmp(current->limiter, line) == 0)
				{
					free(line);
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
				if (fd != -1 && current->next == NULL)
				{
					write(fd, line, ft_strlen(line));
					write(fd, "\n", 1);
				}
				free(line);
			}
			current = current->next;
		}
	}
	if (fd != -1)
		close(fd);
	free_heredocs(redirect->heredoc);
	redirect->heredoc = NULL;
	if (pid == 0)
	{
		free(redirect->infile);
		free(redirect->outfile);
		free(redirect);
		ft_freetab(to_free.cmds);
		free_cmds_parsed(to_free.cmds_parsed);
		free(to_free.cmd);
		closing_the_program(environment);
		exit(0);
	}
	else
		wait(NULL);
}

t_redirect	*redirections(char *line, int empty, t_free to_free)
{
	t_redirect	*redirect;
	int			leftreturn;
	int			rightreturn;

	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	redirect->infile = NULL;
	redirect->heredoc = NULL;
	redirect->heredocfile = NULL;
	redirect->outfile = NULL;
	redirect->append = 0;
	if (!empty)
	{
		redirect->to_execute = TRUE;
		leftreturn = _leftchevron(line, redirect);
		if (leftreturn == -1)
			return (free_redirect(redirect), NULL);
		else if (leftreturn == 0)
		{
			redirect->to_execute = FALSE;
			free_heredocs(redirect->heredoc);
			redirect->heredoc = NULL;
		}
		rightreturn = _rightchevron(line, redirect);
		if (rightreturn == -1)
			return (free_redirect(redirect), NULL);
		else if (leftreturn == 0)
			redirect->to_execute = FALSE;
		use_heredoc(redirect, to_free);
	}
	else
	{
		redirect->to_execute = FALSE;
		line[0] = '\0';
	}
	return (redirect);
}
