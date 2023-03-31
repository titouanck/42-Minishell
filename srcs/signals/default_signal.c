/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:53:47 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/27 16:09:39 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_default_sigint(int sig)
{
	(void) sig;
	g_returnval = 130;
	if (use_readline())
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
}

void	default_signal_tty(void)
{
	signal(SIGINT, _default_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	_notatty_sigint(int sig)
{
	(void) sig;
	g_returnval = 130;
	closing_the_program(NULL);
	exit(g_returnval);
}

static void	_no_action(int sig)
{
	(void) sig;
	return ;
}

void	default_signal_notty(void)
{
	signal(SIGINT, _notatty_sigint);
	signal(SIGQUIT, _no_action);
}
