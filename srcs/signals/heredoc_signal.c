/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:00:43 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
