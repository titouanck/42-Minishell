/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_argv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:50:31 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 19:52:03 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	use_argv(t_env *environment, int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc < 3 || ft_strncmp(argv[1], "-c", 3))
		return (0);
	fd = open("/dev/null", O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror("open");
		g_returnval = errno;
	}
	else
	{
		dup2(fd, 0);
		close(fd);
		(environment->line_nbr)++;
		environment->heredoc_files = NULL;
		line = db_strdup(argv[2]);
		environment->line = line;
		environment->last_input = db_strdup(line);
		parsing(environment, &line);
		rm_heredoc_files(environment);
	}
	return (1);
}
