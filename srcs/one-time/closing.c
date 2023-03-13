/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/13 11:54:17 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_logfile(int fd, char **args, char *last_input)
{
	int		db_size;
	size_t	i;

	dup2(fd, STDOUT_FILENO);
	if (args)
	{
		ft_printf("Child procces terminated with exit code (%d)\n", g_returnval);
		if (args[0])
		{
			ft_printf("cmd:\t\"%s\"\n", args[0]);
			if (args[1])
				ft_printf("args:\t");
			i = 1;
			while (args[i])
			{
				ft_printf("\"%s\"", args[i++]);
				if (args[i])
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
	db_freetab(args);
	db_free(last_input);
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
	char	**args;
	char	*last_input;
	int		fd;

	rl_clear_history();
	args = NULL;
	last_input = NULL;
	if (environment)
	{
		db_free(environment->line);
		args = environment->args;
		last_input = environment->last_input;
		ft_free_environment(environment);
	}
	file = ft_randomstr(".logs/minsh_", ".log", 16);
	if (file)
	{
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		free(file);
		if (fd != -1)
		{
			_logfile(fd, args, last_input);
		}
		else
		{
			perror("minishell: open");
			db_freetab(args);
			db_free(last_input);
		}
	}
	dynamic_memory_address_db(ADDRESSDB_ERASURE, NULL);
}
