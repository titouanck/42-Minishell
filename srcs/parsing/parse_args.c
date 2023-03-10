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

void	print_redirection(char *line, t_redirect *redirect)
{
	t_heredoc *heredoc;

	printf("line: ");
	for (size_t i = 0; line[i]; i++)
	{
		if (line[i] > 0)
			printf(RED"%c"ENDCL, line[i]);
		else if (line[i + 1] > 0)
			printf(RED" "ENDCL);
	}
	printf("\ninfile: "RED"%s"ENDCL"\n", redirect->infile);
	printf("outfile: "RED"%s"ENDCL"\n", redirect->outfile);
	printf("append: "RED"%d"ENDCL"\n", redirect->append);
	printf("Heredocs: ");
	heredoc = redirect->heredoc;
	while (heredoc)
	{
		printf(RED"%s"ENDCL, heredoc->limiter);
		if (heredoc->next)
			printf(" -> ");
		heredoc = heredoc->next;
	}
	printf("\n");
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
		if (args[i][j] == QUOTES)
		{
			ft_memmove(args[i] + j, args[i] + j + 1, ft_strlen(args[i] + j + 1) + 1);
			i--;
		}
		i++;
	}
}

t_cmd	*parse_args(t_env *environment, char **line)
{
	t_cmd		*cmd;

	cmd = db_malloc(sizeof(t_cmd));
	if (!cmd)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	cmd->args = NULL;
	cmd->redirect = NULL;
	if (!quotes_interpretation(environment, line))
	{
		return (db_free(cmd), NULL);
	}
	else
		cmd->redirect = redirections(environment, *line, FALSE);
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
