/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:18:27 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/15 17:13:52 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	use_readline(void)
{
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		return (TRUE);
	else
		return (FALSE);
}

void	rm_heredoc_files(t_env *environment)
{
	char	**cmd;
	char	**heredoc_files;
	char	*tmp;
	size_t	i;
	pid_t	pid;

	heredoc_files = ft_split(environment->heredoc_files, '|');
	if (!heredoc_files)
		return ;
	i = 0;
	while (heredoc_files[i])
	{
		pid = fork();
		if (pid == 0)
		{
			tmp = ft_strjoin("/usr/bin/rm rm -f ", heredoc_files[i]);
			cmd = ft_split(tmp, ' ');
			free(tmp);
			if (cmd)
			{
				execve(cmd[0], cmd + 1, NULL);
				ft_freetab(cmd);
			}
			tmp = ft_strjoin("/bin/rm rm -f ", heredoc_files[i]);
			cmd = ft_split(tmp, ' ');
			free(tmp);
			if (cmd)
			{
				execve(cmd[0], cmd + 1, NULL);
				ft_freetab(cmd);
			}
			ft_freetab(heredoc_files);
			exit(1);
		}
		else
			wait(NULL);
		i++;
	}
	ft_freetab(heredoc_files);
}

int	main(int argc, char **argv, char *envp[]) 
{
	char	*line;
	t_env	*environment;
	int		tty;

	environment = opening(argc, argv, envp);
	if (!environment)
		return (closing_the_program(NULL), 42);
	environment->line = NULL;
	environment->last_input = NULL;
	environment->line_nbr = 0;
	environment->log.args = NULL;
	environment->log.infile = NULL;
	environment->log.outfile = NULL;
	environment->prompt = db_strdup("\r\r\001"GREEN"●"ENDCL"\002 \001\033[34;1m\002minishell \001→\002 \001\033[0m\002");
	if (!environment->prompt)
	{
		g_returnval = 1;
		closing_the_program(environment);
		ft_putstr_fd(ERRALLOC, 2);
		return (g_returnval);
	}
	while (++(environment->line_nbr))
	{
		environment->heredoc_files = NULL;
		if (g_returnval == 0)
			environment->prompt[6] = '2';
		else
			environment->prompt[6] = '1';
		tty = -1;
		if (use_readline())
		{
			if (tty != 1)
			{
				default_signal_behavior();
				tty = 1;
			}
			line = readline(environment->prompt);
			dynamic_memory_address_db('+', line);
		}
		else
		{
			if (tty != 0)
			{
				notatty_signal_behavior();
				tty = 0;
			}
			line = get_next_line(0);
			if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		// size_t	i;
		// i = 0;
		// // dup2(2, 1);
		// while (line[i])
		// {
		// 	printf("line[%d] = (%c) ou (%d)\n", i, line[i], line[i]);
		// 	i++;
		// }
		
		if (line && *line)
			add_history(line);
		if (!line)
			break ;
		db_free(environment->line);
		db_free(environment->last_input);
		environment->line = line;
		environment->last_input = db_strdup(line);
		parsing(environment, &line);
		rm_heredoc_files(environment);
	}
	closing_the_program(environment);
	if (use_readline())
		ft_putstr_fd("exit\n", 2);
	return (g_returnval);
}
