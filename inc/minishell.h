/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:29 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 16:34:58 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define PROMPT "minishell →"
# define ERRALLOC "minishell: Could not allocate memory.\n"
# define ERRPIPE "minishell: syntax error near unexpected `|\'\n"
# define VARNAMESET "abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789_"

# define PIPECHAR -1
# define VARKEY -2
# define SEPARATOR -3
# define LEFTCHEVRON -4
# define RIGHTCHEVRON -5
# define HEREDOC -6
# define EMPTYQUOTE -7
# define NOTAVARKEY -8

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef	struct	s_heredoc
{
	char				*limiter;
	struct s_heredoc	*next;
}						t_heredoc;

typedef struct	s_redirect
{
	char		*infile;
	int			fd_infile;
	t_heredoc	*heredoc;
	char		*heredocfile;
	char		*outfile;
	int			fd_outfile;
	int			append;
	int			to_execute;
}			t_redirect;

typedef struct s_cmd
{
	char		**args;
	t_redirect	*redirect;
	int			saved_stdout;
	int			saved_stdin;
}				t_cmd;

typedef struct s_free
{
	char	**cmds;
	t_cmd	**cmds_parsed;
	t_cmd	*cmd;
}			t_free;

typedef struct s_env
{
	int				g_returnval;
	char			*key;
	char			*value;
	int				exported;
	int				limiter_between_quotes;
	struct s_env	*next;
	t_free			to_free;
}					t_env;

extern t_env	*environment;

			/* Built-in Functions & Commands */

void		ftbuiltin_echo(char **args);
void		ftbuiltin_cd(char **args);
void		ftbuiltin_pwd();
void		ftbuiltin_export(char **args);
void		ftbuiltin_unset(char **args);
void		ftbuiltin_env();
void		ftbuiltin_exit(char **args, t_cmd **cmds, size_t cmdnbr);

// execute_cmd.c
int		execute_cmd(char **args);

					/* Parsing */

// parse_builtin.c
int	parse_builtin(char **args, \
	t_cmd **cmds, size_t cmdnbr);
// split_cmds.c
char		**split_cmds(char **ptr);
// parsing.c
void		parsing(char **line);
// parse_cmd.c
t_cmd		*parse_cmd(char **line, t_free to_free);
// parse_args.c
t_cmd		*parse_args(char **line, t_free to_free);
// redirections.c
t_redirect	*redirections(char *line, int empty, t_free to_free);
// replace_key_by_value.c
char		*replace_key_by_value(char *line);
// quotes_interpretation.c
int			quotes_interpretation(char **line);

				/* One-time actions */

// opening.c
t_env		*opening(int argc, char **argv, char *envp[]);
// closing.c
void		closing_the_program();
// signal.c
void		default_signal_behavior(void);
void		heredoc_signal_behavior(void);
void		cmd_signal_behavior(void);
// get_environment.c
t_env		*get_environment(char *envp[]);

					/* Environment */

// ftbuiltin_unset.c
void		ftbuiltin_unset_element(char *arg);
// change_local_variables.c
int			change_local_variables(char *line, size_t size);
// environment.c
char		**format_environment();
void		free_environment();
char		*get_value_by_key(char *key);
int			env_lstaddback(char *key, char *value, int exported);
// path.c
char		**get_path(char *envp[]);

					/* Pipex */

int		pipex(char **cmds);
int		first_child(int pipefd[2], t_cmd **cmds);
int		middle_child(int pipefd[2], t_cmd **cmds, size_t cmdnbr);
void	last_child(int pipefd[2], t_cmd **cmds, size_t cmdnbr);
// io_open_fds.c
int		io_open_fds(t_redirect *redirect);

// PAS CLASSE
void	free_redirect(t_redirect *redirect);
void	free_cmds_parsed(t_cmd **tab);

					/* Utils */
void	heredoc_signal_behavior_parent(void);

#endif
