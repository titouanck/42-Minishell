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

void	free_redirect(t_redirect *redirect)
{
	if (!redirect)
		return ;
	free(redirect->infile);
	free(redirect->outfile);
	free(redirect);
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
	free_redirect(redirect);
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
