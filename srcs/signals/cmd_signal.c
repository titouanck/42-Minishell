/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:58:02 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 12:01:42 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_cmd_sigint(int sig)
{
	(void) sig;
	ft_putchar_fd('\n', 2);
	return ;
}

void	cmd_signal_child(void)
{
	signal(SIGINT, _cmd_sigint);
	signal(SIGQUIT, SIG_DFL);
}

static void	_cmd_sigquit(int sig)
{
	(void) sig;
	return ;
}

void	cmd_signal_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, _cmd_sigquit);
	rl_on_new_line();
}
