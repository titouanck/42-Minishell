/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_to_logfile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:15:13 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_child_process(int fd, t_log log)
{
	size_t	i;

	ft_putstr_fd("Child procces terminated with exit code (", fd);
	ft_putnbr_fd(g_returnval, fd);
	ft_putstr_fd(")\n", fd);
	if (log.args[0])
	{
		ft_putstr_fd("cmd:\t\"", fd);
		ft_putstr_fd(log.args[0], fd);
		ft_putstr_fd("\"\n", fd);
		if (log.args[1])
			ft_putstr_fd("args:\t", fd);
		i = 1;
		while (log.args[i])
		{
			ft_putchar_fd('\"', fd);
			ft_putstr_fd(log.args[i++], fd);
			ft_putchar_fd('\"', fd);
			if (log.args[i])
				ft_putstr_fd(", ", fd);
			else
				ft_putstr_fd("\n", fd);
		}
	}
}

static void	_parent_process(int fd, char *last_input)
{
	ft_putstr_fd("Parent procces terminated with exit code (", fd);
	ft_putnbr_fd(g_returnval, fd);
	ft_putstr_fd(")\n", fd);
	ft_putstr_fd("last input: \"", fd);
	ft_putstr_fd(last_input, fd);
	ft_putstr_fd("\"\n", fd);
}

static void	_redirections(int fd, t_log log)
{
	ft_putchar_fd('\n', fd);
	if (log.infile)
	{
		ft_putstr_fd("in  :\t\"", fd);
		ft_putstr_fd(log.infile, fd);
		ft_putstr_fd("\"\n", fd);
	}
	if (log.outfile)
	{
		ft_putstr_fd("out :\t\"", fd);
		ft_putstr_fd(log.outfile, fd);
		ft_putstr_fd("\"\n", fd);
	}
	db_free(log.infile);
	db_free(log.outfile);
}

void	write_to_logfile(int fd, t_log log, char *last_input)
{
	int		db_size;

	if (log.args)
		_child_process(fd, log);
	else
		_parent_process(fd, last_input);
	db_freetab(log.args);
	db_free(last_input);
	if (log.infile || log.outfile)
		_redirections(fd, log);
	db_size = dynamic_memory_address_db(ADDRESSDB_SIZE, NULL);
	ft_putchar_fd('\n', fd);
	if (db_size > 0)
	{
		ft_putnbr_fd(db_size, fd);
		ft_putstr_fd(" memory addresses were not freed manually :\n", fd);
	}
	else
		ft_putstr_fd("All memory addresses seem to have been freed.\n", fd);
	dynamic_memory_address_db(ADDRESSDB_PRINT, &fd);
	close(fd);
}
