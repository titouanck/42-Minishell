/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/27 17:52:19 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	closing_the_program(t_env *environment)
{
	int		fd_stdincp;
	int		fd_stdoutcp;
	char	*tmp;

	(void) tmp;
	(void) fd_stdincp;
	(void) fd_stdoutcp;
	rl_clear_history();
	tmp = get_value_by_key(environment, "@!stdincp");
	if (tmp)
	{
		fd_stdincp = ft_atoi(tmp);
		close(fd_stdincp);
	}
	tmp = get_value_by_key(environment, "@!stdoutcp");
	if (tmp)
	{
		fd_stdoutcp = ft_atoi(tmp);
		close(fd_stdoutcp);
	}
	free_environment(environment);
}
