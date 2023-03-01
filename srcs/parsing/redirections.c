/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 18:37:34 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_heredoc *lstnew_heredoc(t_heredoc *heredoc, char *limiter)
{
	t_heredoc	*new;
	t_heredoc	*current;

	new = malloc(sizeof(heredoc));
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

char	*heredoc_file(void)
{
	int				fd;
	unsigned char	buf[16];
	char			*filename;
	size_t			i;

	filename = ft_strdup(".tmpheredocfile_____.minishell");
	if (!filename)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1 || read(fd, buf, sizeof(buf)) != sizeof(buf))
		return (filename);
	close(fd);
	i = -1;
	while (++i < 16)
	{
		if (buf[i] > 127)
			buf[i] = buf[i] / 2 - 1;
		if (buf[i] < 33)
			buf[i] += 33;
		else if (buf[i] == 127)
			buf[i] -= 1;
		filename[i + 4] = buf[i];
	}
	return (filename);
}

void	use_heredoc(t_redirect *redirect)
{
	t_heredoc	*current;
	char		*line;

	if (!redirect || !redirect->heredoc)
		return ;
	current = redirect->heredoc;
	while (current)
	{
		while (1)
		{
			line = get_next_line(0);
			ft_putstr_fd(line, 1);
			if (!line)
				break ;
			if (!(*line))
			{
				free(line);
				break ;
			}
			free(line);
		}
		// if (redirect->infile == NULL && redirect->heredoc)
		// 	redirect->infile = heredoc_file();
		current = current->next;
	}
	
}

t_redirect	*redirections(char *line)
{
	t_redirect	*redirect;

	(void) line;
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
