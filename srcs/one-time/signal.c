/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/14 18:42:31 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_default_sigint(int sig)
{
	(void) sig;
	g_returnval = 130;
	if (use_readline())
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
} 

void	default_signal_behavior(void)
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

void	_no_action(int sig)
{
	(void) sig;
	return ;
}

void	notatty_signal_behavior(void)
{
	signal(SIGINT, _notatty_sigint);
	signal(SIGQUIT, _no_action);
}

static void	_cmd_sigint(int sig)
{
	(void) sig;
	ft_putchar_fd('\n', 2);
	return ;
} 

static void	_cmd_sigquit(int sig)
{
	(void) sig;
	ft_putstr_fd("Quit (core dumped)\n", 2);
	g_returnval = 131;
	return ;
} 

void	cmd_signal_behavior(void)
{
	signal(SIGINT, _cmd_sigint);
	signal(SIGQUIT, SIG_DFL);
}


void	cmd_signal_behavior_parent(void)
{
	signal(SIGINT, _cmd_sigint);
	signal(SIGQUIT, _cmd_sigquit);
}

static void	_heredoc_sigint(int sig)
{
	(void) sig;
	g_returnval = 130;
	rl_done = 1;
	rl_redisplay();
}

void	heredoc_signal_behavior(void)
{
	if (!use_readline())
		signal(SIGINT, SIG_DFL);
	else
		signal(SIGINT, _heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}
