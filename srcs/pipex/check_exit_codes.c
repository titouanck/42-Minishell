/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit_codes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 12:03:11 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 16:13:48 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_action_on_sigquit(t_cmd **cmds)
{
	size_t	i;

	if (!cmds || !cmds[0] || !cmds[1])
		return (1);
	i = 0;
	while (cmds[i + 1])
		i++;
	if (waitpid(cmds[i]->pid, NULL, WNOHANG) == 0)
		return (1);
	return (0);
}

static void	_action_on_status(t_cmd **cmds, int status, int init)
{
	static int	sig_quit;

	if (init)
	{
		sig_quit = 0;
		return ;
	}
	if (status == 131 && !sig_quit)
	{
		if (_action_on_sigquit(cmds))
			ft_putstr_fd("Quit (core dumped)\n", 2);
		sig_quit = 1;
	}
	if (status == 2)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

int	check_exit_codes(t_cmd **cmds)
{
	int		status;
	size_t	i;

	_action_on_status(NULL, 0, TRUE);
	if (!cmds || !(*cmds))
		return (0);
	status = 0;
	i = 0;
	while (cmds[i])
	{
		waitpid(cmds[i]->pid, &status, 0);
		_action_on_status(cmds + i, status, FALSE);
		if (status == 2 && !use_readline())
			return (status);
		i++;
	}
	return (status);
}
