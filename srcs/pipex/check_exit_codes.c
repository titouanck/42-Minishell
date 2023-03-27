/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit_codes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:03:11 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/27 15:52:08 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_exit_codes(t_cmd **cmds)
{
	int		sigint;
	int		status;
	size_t	i;

	if (!cmds || !(*cmds))
		return (0);
	sigint = 0;
	status = 0;
	i = -1;
	while (cmds[++i])
	{
		waitpid(cmds[i]->pid, &status, 0);
		if (status == 131 && !cmds[i + 1])
			ft_putstr_fd("Quit (core dumped)\n", 2);
		if (status == 2 && sigint++ == 0)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_done = 1;
			if (!use_readline())
				break ;
		}
	}
	return (status);
}
