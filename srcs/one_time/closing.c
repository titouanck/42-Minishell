/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:15:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_openwrite_to_logfile(char *last_input, t_log log)
{
	char	*file;
	int		fd;

	file = ft_randomstr("/tmp/.minishell-logs/minishell-log_", ".log", 16);
	if (file)
	{
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		free(file);
		if (fd != -1)
			write_to_logfile(fd, log, last_input);
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
}

void	closing_the_program(t_env *environment)
{
	char	*last_input;
	t_log	log;

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
	_openwrite_to_logfile(last_input, log);
	dynamic_memory_address_db(ADDRESSDB_ERASURE, NULL);
}
