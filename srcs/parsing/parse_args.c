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
				ft_memmove(args[i] + j, args[i] + j + 1, ft_strlen(args[i] + j + 1) + 1);
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
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	cmd->args = NULL;
	cmd->redirect = NULL;
	if (!quotes_interpretation(environment, line))
		return (db_free(cmd), NULL);
	else
		cmd->redirect = redirections(environment, *line, last);
	if (!(cmd->redirect))
		return (db_free(cmd), NULL);
	cmd->args = db_split(*line, SEPARATOR);
	if (!(cmd->args))
	{
		ft_putstr_fd(ERRALLOC, 2);
		g_returnval = 12;
		db_free(*line);
		ft_free_redirect(cmd->redirect);
		db_free(cmd);
		closing_the_program(environment);
		exit(g_returnval);
	}
	_remove_quote_token_tab(cmd->args);
	return (cmd);
}
