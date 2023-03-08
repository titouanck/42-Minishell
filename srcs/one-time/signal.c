/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 17:23:29 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_default_sigint(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
} 

void	default_signal_behavior(void)
{
	signal(SIGINT, _default_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	_cmd_sigint(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	return ;
} 

void	cmd_signal_behavior(void)
{
	signal(SIGINT, _cmd_sigint);
	signal(SIGQUIT, SIG_DFL);
}

static void	_heredoc_sigint(int sig)
{
	(void) sig;
	g_returnval = 130;
	rl_done = 1;
}

void	heredoc_signal_behavior(void)
{
	signal(SIGINT, _heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}
