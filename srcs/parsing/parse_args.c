/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:03:48 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 16:19:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_remove_arg(char **args, size_t rm_i, size_t j)
{
	size_t	i;
	size_t	start_j;

	start_j = j++;
	if (!ft_isdigit(args[rm_i][j]))
		while (args[rm_i][j] && ft_inset(args[rm_i][j], VARNAMESET))
			j++;
	else
		j += 1;
	ft_memmove(args[rm_i] + start_j, args[rm_i] + j, \
	ft_strlen(args[rm_i] + j) + 1);
	if (ft_strlen(args[rm_i]) > 0)
		return (0);
	db_free(args[rm_i]);
	args[rm_i] = NULL;
	i = rm_i + 1;
	while (1)
	{
		args[i - 1] = args[i];
		if (!args[i])
			break ;
		i += 1;
	}
	return (1);
}

static void	_remove_emptyvar_token_tab(char **args)
{
	size_t	i;
	size_t	j;

	if (!args)
		return ;
	i = -1;
	while (args[++i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == EMPTYVARIABLE)
			{
				if (_remove_arg(args, i, j))
				{
					i--;
					break ;
				}
				j--;
			}
			j++;
		}
	}
}

static void	_remove_quote_token_tab(char **args)
{
	size_t	i;
	size_t	j;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == QUOTES)
			{
				ft_memmove(args[i] + j, args[i] + j + 1, \
				ft_strlen(args[i] + j + 1) + 1);
				j--;
			}
			j++;
		}
		i++;
	}
}

t_cmd	*parse_args(t_env *environment, char **line, int last)
{
	t_cmd		*cmd;

	cmd = db_malloc(sizeof(t_cmd));
	if (!cmd)
		exit_erralloc(environment);
	cmd->args = NULL;
	cmd->redirect = NULL;
	if (!quotes_interpretation(environment, line))
		return (db_free(cmd), NULL);
	cmd->redirect = redirections(environment, *line, last);
	if (!(cmd->redirect))
		return (db_free(cmd), NULL);
	cmd->args = db_split(*line, SEPARATOR);
	if (!(cmd->args))
	{
		exit_erralloc(environment);
		g_returnval = 12;
		db_free(*line);
		ft_free_redirect(cmd->redirect);
		db_free(cmd);
		closing_the_program(environment);
		exit(g_returnval);
	}
	return (_remove_emptyvar_token_tab(cmd->args), \
	_remove_quote_token_tab(cmd->args), cmd);
}
