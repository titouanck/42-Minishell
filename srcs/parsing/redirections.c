/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 20:06:56 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_redirect(t_redirect *redirect)
{
	if (!redirect)
		return ;
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
					return (ft_putstr_fd("minishell: syntax error near unexpected token `\? < \?\'\n", 2), 0);
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
				limiter = ft_strdupn(start, end - start);
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
					return (ft_putstr_fd("minishell: syntax error near unexpected token `\? < \?\'\n", 2), 0);
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
				redirect->infile = ft_strdupn(start, end - start);
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
				return (ft_putstr_fd("minishell: syntax error near unexpected token `\? > \?\'\n", 2), 0);
			start = line + i;
			while (line[i] > 0)
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
				free(redirect->outfile);
			}
			redirect->outfile = ft_strdupn(start, end - start);
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
		perror("open");
		free(filename);
		redirect->infile = NULL;
	}
	return (fd);
}

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

void	use_heredoc(t_redirect *redirect)
{
	t_heredoc	*current;
	char		*line;
	int			fd;

	if (!redirect || !redirect->heredoc)
		return ;
	fd = -1;
	if (redirect->infile == NULL && redirect->heredoc)
		fd = heredoc_file(redirect);
	current = redirect->heredoc;
	while (current)
	{
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_putstr_fd("ERREUR\n", 2);
				break ;
			}
			if (ft_strcmp(current->limiter, line) == 0)
			{
				free(line);
				break ;
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
	if (fd != -1)
		close(fd);
	free_heredocs(redirect->heredoc);
	redirect->heredoc = NULL;
}

t_redirect	*redirections(char *line)
{
	t_redirect	*redirect;

	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	redirect->infile = NULL;
	redirect->heredoc = NULL;
	redirect->outfile = NULL;
	redirect->append = 0;
	if (!_leftchevron(line, redirect) || !_rightchevron(line, redirect))
		return (free(redirect->infile), free(redirect->outfile), free(redirect->heredoc), free(redirect), NULL);
	use_heredoc(redirect);
	return (redirect);
}
