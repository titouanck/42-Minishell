/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 19:37:07 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define POSITIVE_MAX 9223372036854775807ULL
#define NEGATIVE_MAX 9223372036854775808ULL

static int	_get_positive_exitcode(char *str, unsigned long long *nbr)
{
	unsigned long long		remaining;
	unsigned long long		next;

	next = *str - '0';
	if (*nbr > 922337203685477580ULL || *nbr * 10 > POSITIVE_MAX)
		return (0);
	remaining = POSITIVE_MAX - (*nbr * 10);
	if (next > remaining)
		return (0);
	remaining -= next;
	*nbr = *nbr * 10 + next;
	return (1);
}

static int	_get_negative_exitcode(char *str, unsigned long long *nbr)
{
	unsigned long long		remaining;
	unsigned long long		next;

	next = *str - '0';
	if (*nbr > 922337203685477580ULL || *nbr * 10 > NEGATIVE_MAX)
		return (0);
	remaining = NEGATIVE_MAX - (*nbr * 10);
	if (next > remaining)
		return (0);
	remaining -= next;
	*nbr = *nbr * 10 + next;
	return (1);
}

static int	_adapted_atoi(char *str, unsigned char *code)
{
	unsigned long long		nbr;
	long long				sign;

	if (!str)
		return (0);
	nbr = 0;
	sign = 1;
	ft_strip(str);
	if (*str == '+')
		str += 1;
	else if (*str == '-' && str++)
		sign *= -1;
	while ('0' <= *str && *str <= '9')
	{
		if (sign == 1 && !_get_positive_exitcode(str, &nbr))
			return (0);
		else if (sign == -1 && !_get_negative_exitcode(str, &nbr))
			return (0);
		str += 1;
	}
	if (*str)
		return (0);
	*code = (unsigned char)((long long)nbr * sign);
	return (1);
}

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
	if (use_readline())
		ft_putstr_fd("exit\n", 2);
	ft_free_cmds_parsed(cmds);
	closing_the_program(environment);
	exit(g_returnval);
}

void	ftbuiltin_exit(t_env *environment, \
	char **args, t_cmd **cmds, size_t cmdnbr)
{
	unsigned char	code;
	char			*tmp_str;

	code = g_returnval;
	if (args && *args && args[1])
	{
		tmp_str = db_strdup(args[1]);
		if (!_adapted_atoi(tmp_str, &code))
		{
			db_free(tmp_str);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			_i_want_to_exit(environment, cmds, 2, cmdnbr);
			return ;
		}
		db_free(tmp_str);
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_returnval = 1;
			return ;
		}
	}
	_i_want_to_exit(environment, cmds, code, cmdnbr);
}
