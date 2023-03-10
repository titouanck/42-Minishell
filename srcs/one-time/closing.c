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
	int	fd;

	rl_clear_history();
	if (environment)
	{
		fd = environment->fd_log;
		ft_free_environment(environment);
	}
	else
		fd = -1;
	if (fd != -1 && (access("assets/", X_OK) == 0 && access("assets/minishell.log", W_OK) == 0))
	{
		dup2(fd, STDOUT_FILENO);
		ft_putstr("Memory addresses still open before closing the program with (");
		ft_putnbr(g_returnval);
		ft_putstr(") :\n");
		dynamic_memory_address_db(ADDRESSDB_PRINT, NULL);
		ft_putchar('\n');
		close(fd);
	}
	dynamic_memory_address_db(ADDRESSDB_ERASURE, NULL);
}
