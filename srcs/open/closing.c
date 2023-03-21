/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 18:49:26 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_logfile(int fd, t_log log, char *last_input)
{
	int		db_size;
	size_t	i;

	if (log.args)
	{
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
	else
	{
		ft_putstr_fd("Parent procces terminated with exit code (", fd);
		ft_putnbr_fd(g_returnval, fd);
		ft_putstr_fd(")\n", fd);
		ft_putstr_fd("last input: \"", fd);
		ft_putstr_fd(last_input, fd);
		ft_putstr_fd("\"\n", fd);
	}
	db_freetab(log.args);
	db_free(last_input);
	if (log.infile || log.outfile)
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

void	closing_the_program(t_env *environment)
{
	char	*file;
	char	*last_input;
	t_log	log;
	int		fd;

	rl_clear_history();
	log.args = NULL;
	log.infile = NULL;
	log.outfile = NULL;
	last_input = NULL;
	if (environment)
	{
		db_free(environment->heredoc_files);
		environment->heredoc_files = NULL;
		db_free(environment->prompt);
		db_free(environment->line);
		log = environment->log;
		last_input = environment->last_input;
		ft_free_environment(environment);
	}
	file = ft_randomstr("/tmp/.minishell-logs/minishell-log_", ".log", 16);
	if (file)
	{
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		free(file);
		if (fd != -1)
			_logfile(fd, log, last_input);
		else
			perror("minishell: open");

	}
	if (!file || fd == -1)
	{
		db_freetab(log.args);
		db_free(log.infile);
		db_free(log.outfile);
		db_free(last_input);
	}
	dynamic_memory_address_db(ADDRESSDB_ERASURE, NULL);
}