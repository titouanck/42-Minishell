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

#include <minishell.h>

void	print_redirection(char *line, t_redirect *redirect)
{
	t_heredoc *heredoc;

	printf("line: %s$\n", line);
	printf("infile: %s$\n", redirect->infile);
	printf("outfile: %s$\n", redirect->outfile);
	printf("append: %d$\n", redirect->append);
	printf("Heredocs: ");
	heredoc = redirect->heredoc;
	while (heredoc)
	{
		printf("%s", heredoc->limiter);
		if (heredoc->next)
			printf(" -> ");
		else
			printf("\n");
		heredoc = heredoc->next;
	}
}

char	**parse_args(t_env *environment, char **line)
{
	char		**args;
	t_redirect	*redirect;

	(void) redirect;
	if (!quotes_interpretation(environment, line))
		return (NULL);
	redirect = redirections(*line);
	if (!redirect)
		return (NULL);
	print_redirection(*line, redirect);
	args = ft_split(*line, SEPARATOR);
	if (!args)
	{
		ft_putstr_fd(ERRALLOC, 2);
		g_returnval = 12;
		free(*line);
		closing_the_program(environment);
		exit(g_returnval);
	}
	return (args);
}
