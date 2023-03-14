/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/14 17:50:05 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_logfile(int fd, t_log log, char *last_input)
{
	int		db_size;
	size_t	i;

	dup2(fd, STDOUT_FILENO);
	if (log.args)
	{
		ft_printf("Child procces terminated with exit code (%d)\n", g_returnval);
		if (log.args[0])
		{
			ft_printf("cmd:\t\"%s\"\n", log.args[0]);
			if (log.args[1])
				ft_printf("args:\t");
			i = 1;
			while (log.args[i])
			{
				ft_printf("\"%s\"", log.args[i++]);
				if (log.args[i])
					ft_putstr(", ");
				else
					ft_putstr("\n");
			}
		}
	}
	else
	{
		ft_printf("Parent procces terminated with exit code (%d)\n", g_returnval);
		ft_printf("last input: \"%s\"\n", last_input);
	}
	db_freetab(log.args);
	db_free(last_input);
	if (log.infile || log.outfile)
		ft_putchar('\n');
	if (log.infile)
		ft_printf("in  :\t\"%s\"\n", log.infile);
	if (log.outfile)
		ft_printf("out :\t\"%s\"\n", log.outfile);
	db_free(log.infile);
	db_free(log.outfile);
	db_size = dynamic_memory_address_db(ADDRESSDB_SIZE, NULL);
	if (db_size > 0)
		ft_printf("\n%d memory addresses were not freed manually :\n", db_size);
	else
		ft_putstr("\nAll memory addresses seem to have been freed.\n");
	dynamic_memory_address_db(ADDRESSDB_PRINT, NULL);
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
		db_free(environment->prompt);
		db_free(environment->line);
		log = environment->log;
		last_input = environment->last_input;
		ft_free_environment(environment);
	}
	file = ft_randomstr(".minishell-logs/minsh_", ".log", 16);
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
