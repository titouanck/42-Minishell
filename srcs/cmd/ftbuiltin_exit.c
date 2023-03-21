/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 17:21:04 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_i_want_to_exit(t_env *environment, t_cmd **cmds, \
unsigned char code, size_t cmdnbr)
{
	size_t	size;

	g_returnval = code;
	size = 0;
	while (cmds[size])
		size++;
	if (size > 1)
		return ;
	if (cmds && cmds[cmdnbr] && (cmds[cmdnbr])->redirect->infile)
	{
		close((cmds[cmdnbr])->redirect->fd_infile);
		dup2(cmds[cmdnbr]->saved_stdin, STDIN_FILENO);
		close(cmds[cmdnbr]->saved_stdin);
	}
	if (cmds && cmds[cmdnbr] && (cmds[cmdnbr])->redirect->outfile)
	{
		close((cmds[cmdnbr])->redirect->fd_outfile);
		dup2(cmds[cmdnbr]->saved_stdout, STDOUT_FILENO);
		close(cmds[cmdnbr]->saved_stdout);
	}
	ft_free_cmds_parsed(environment, cmds);
	closing_the_program(environment);
	if (use_readline())
		ft_putstr_fd("exit\n", 2);
	exit(g_returnval);
}

int	my_atoi(const char *str, unsigned char *code)
{
	unsigned long long		nbr;
	unsigned long long		positive_remaining;
	unsigned long long		negative_remaining;
	unsigned long long		next;
	long long				sign;
	size_t					i;

	nbr = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r'))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		next = str[i] - '0';
		if (sign == 1)
		{
			positive_remaining = 9223372036854775807ULL - (nbr * 10);
			if (next > positive_remaining)
				return (0);
			positive_remaining -= next;
		}
		else
		{
			negative_remaining = 9223372036854775808ULL - (nbr * 10);
			if (next > negative_remaining)
				return (0);
			negative_remaining -= next;
		}
		nbr = nbr * 10 + next;
		i++;
	}
	while (str[i])
	{
		if (!ft_iswhitespace(str[i]))
			return (0);
		i++;
	}
	*code = (unsigned char) ((long long)nbr * sign);
	positive_remaining = 42ULL - nbr;
	return (1);
}

void	ftbuiltin_exit(t_env *environment, char **args, t_cmd **cmds, size_t cmdnbr)
{
	unsigned char	code;
	char			*errmsg;
	size_t			i;

	code = g_returnval;
	if (args && *args && args[1])
	{
		i = 1;
		if (!my_atoi(args[1], &code))
		{
			errmsg = db_strrjoin("minishell: exit: ", args[1], ": numeric argument required\n");
			if (!errmsg)
				exit_erralloc(environment);
			else
				ft_putstr_fd(errmsg, 2);
			db_free(errmsg);
			_i_want_to_exit(environment, cmds, 2, cmdnbr);
			return ;
		}
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_returnval = 1;
			return ;
		}
	}
	_i_want_to_exit(environment, cmds, code, cmdnbr);	
}
