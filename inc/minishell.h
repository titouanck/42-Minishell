/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:29 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/27 17:40:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define PROMPT "minishell →"
# define ERRALLOC "minishell: Could not allocate memory.\n"
# define ERRPIPE "minishell: syntax error near unexpected token `|\'\n"
# define VARNAMESET "abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789_"

# define PIPECHAR -1
# define VARKEY -2
# define SEPARATOR -3

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

extern int	g_returnval;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
	char			*pwd;
}					t_env;

			/* Built-in Functions & Commands */

void	ftbuiltin_echo(char **args);
void	ftbuiltin_cd(t_env *environment, char **args);
void	ftbuiltin_pwd(t_env *environment);
void	ftbuiltin_export(t_env *environment, char **args);
void	ftbuiltin_unset(t_env *environment, char **args);
void	ftbuiltin_env(t_env *environment);
void	ftbuiltin_exit(t_env *environment, char **args, char **cmds);

// execute_cmd.c
int		execute_cmd(t_env *environment, char **args);

					/* Parsing */

// parse_builtin.c
int	parse_builtin(t_env *environment, char **args, \
	char **cmds, size_t cmdnbr);
// split_cmds.c
char	**split_cmds(char **ptr);
// parsing.c
void	parsing(t_env *environment, char **line);
// parse_cmd.c
char	**parse_cmd(t_env *environment, char **line);
// parse_args.c
char	**parse_args(t_env *environment, char **line);
// replace_key_by_value.c
char	*replace_key_by_value(t_env *environment, char *line);
// quotes_interpretation.c
int		quotes_interpretation(t_env *environment, char **line);

				/* One-time actions */

// opening.c
t_env	*opening(int argc, char **argv, char *envp[]);
// closing.c
void	closing_the_program(t_env *environment);
// signal.c
void	change_signal_behavior(void);
// get_environment.c
t_env	*get_environment(char *envp[]);

					/* Environment */

// ftbuiltin_unset.c
void	ftbuiltin_unset_element(t_env *environment, char *arg);
// change_local_variables.c
int	change_local_variables(t_env *environment, char *line, size_t size);
// environment.c
char	**format_environment(t_env *environment);
void	free_environment(t_env *environment);
char	*get_value_by_key(t_env *environment, char *key);
int		env_lstaddback(t_env *env, char *key, char *value, int exported);
// path.c
char	**get_path(char *envp[]);

					/* Pipex */

void	pipex(t_env *environment, char **cmds);
int		first_child(t_env *environment, int pipefd[2], char **cmds);
int		middle_child(t_env *environment, int pipefd[2], char **cmds, size_t cmdnbr);
void	last_child(t_env *environment, int pipefd[2], char **cmds, size_t size);

#endif
