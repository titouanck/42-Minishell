/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/01 14:58:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	closing_the_program(t_env *environment)
{
	char	*file;
	int		fd;

	rl_clear_history();
	if (environment)
		ft_free_environment(environment);
	file = ft_randomstr(".logs/", ".log", 16);
	if (file)
	{
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			ft_putstr("Procces terminated with exit code (");
			ft_putnbr(g_returnval);
			ft_putstr(")\n");
			ft_putnbr(dynamic_memory_address_db(ADDRESSDB_SIZE, NULL));
			ft_putstr(" memory addresses were not free :\n\n");
			dynamic_memory_address_db(ADDRESSDB_PRINT, NULL);
			close(fd);
		}
		else
			perror("minishell: open");
	}
	dynamic_memory_address_db(ADDRESSDB_ERASURE, NULL);
}
