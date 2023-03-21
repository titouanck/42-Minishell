/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 15:19:36 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_logfile(int fd, t_log log, char *last_input)
{
	int		db_size;
	size_t	i;

	// dup2(fd, STDOUT_FILENO);
	if (log.args)
	{
		ft_putstr_fd("Child procces terminated with exit code (", fd);
		ft_putnbr_fd(g_returnval, fd);
		ft_putstr_fd(")\n", fd);
		// ft_printf("Child procces terminated with exit code (%d)\n", g_returnval);
		if (log.args[0])
		{
			ft_putstr_fd("cmd:\t\"", fd);
			ft_putstr_fd(log.args[0], fd);
			ft_putstr_fd("\"\n", fd);
			// ft_printf("cmd:\t\"%s\"\n", log.args[0]);
			if (log.args[1])
				ft_putstr_fd("args:\t", fd);
				// ft_printf("args:\t");
			i = 1;
			while (log.args[i])
			{
				ft_putchar_fd('\"', fd);
				ft_putstr_fd(log.args[i++], fd);
				ft_putchar_fd('\"', fd);
				// ft_printf("\"%s\"", log.args[i++]);
				if (log.args[i])
					ft_putstr_fd(", ", fd);
					// ft_putstr(", ");
				else
					ft_putstr_fd("\n", fd);
					// ft_putstr("\n");
			}
		}
	}
	else
	{
		ft_putstr_fd("Parent procces terminated with exit code (", fd);
		ft_putnbr_fd(g_returnval, fd);
		ft_putstr_fd(")\n", fd);
		// ft_printf("Parent procces terminated with exit code (%d)\n", g_returnval);
		ft_putstr_fd("last input: \"", fd);
		ft_putstr_fd(last_input, fd);
		ft_putstr_fd("\"\n", fd);
		// ft_printf("last input: \"%s\"\n", last_input);
	}
	db_freetab(log.args);
	db_free(last_input);
	if (log.infile || log.outfile)
		ft_putchar_fd('\n', fd);
		// ft_putchar('\n');
	if (log.infile)
		ft_putstr_fd("in  :\t\"", fd);
		ft_putstr_fd(log.infile, fd);
		ft_putstr_fd("\"\n", fd);
		// ft_printf("in  :\t\"%s\"\n", log.infile);
	if (log.outfile)
		ft_putstr_fd("out :\t\"", fd);
		ft_putstr_fd(log.outfile, fd);
		ft_putstr_fd("\"\n", fd);
		// ft_printf("out :\t\"%s\"\n", log.outfile);
	db_free(log.infile);
	db_free(log.outfile);
	db_size = dynamic_memory_address_db(ADDRESSDB_SIZE, NULL);
	ft_putchar_fd('\n', fd);
	if (db_size > 0)
	{
		ft_putnbr_fd(db_size, fd);
		ft_putstr_fd(" memory addresses were not freed manually :\n", fd);
		// ft_printf("\n%d memory addresses were not freed manually :\n", db_size);
	}
	else
		ft_putstr_fd("All memory addresses seem to have been freed.\n", fd);
		// ft_putstr("\nAll memory addresses seem to have been freed.\n");
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
