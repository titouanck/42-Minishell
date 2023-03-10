/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:29 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/13 11:50:56 by tchevrie         ###   ########.fr       */
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
# define NOTAVARKEY -7
# define QUOTES -8

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

extern int	g_returnval;

typedef struct s_env
{
	char			*line;
	size_t			line_nbr;
	char			**args;
	char			*last_input;
	char			*key;
	char			*value;
	int				exported;
	int				limiter_between_quotes;
	struct s_env	*next;
}					t_env;

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

			/* Built-in Functions & Commands */

void		ftbuiltin_echo(char **args);
void		ftbuiltin_cd(t_env *environment, char **args);
void		ftbuiltin_pwd(t_env *environment);
void		ftbuiltin_export(t_env *environment, char **args);
void		ftbuiltin_unset(t_env *environment, char **args);
void		ftbuiltin_env(t_env *environment);
void		ftbuiltin_exit(t_env *environment, char **args, t_cmd **cmds, size_t cmdnbr);
void		ftbuiltin_dma_db(void);

// execute_cmd.c
int		execute_cmd(t_env *environment, char **args);

					/* Parsing */

// parse_builtin.c
int	parse_builtin(t_env *environment, char **args, \
	t_cmd **cmds, size_t cmdnbr);
// split_cmds.c
char		**split_cmds(t_env *environment, char **ptr);
// parsing.c
void		parsing(t_env *environment, char **line);
// parse_cmd.c
t_cmd		*parse_cmd(t_env *environment, char **line);
// parse_args.c
t_cmd		*parse_args(t_env *environment, char **line);
// redirections.c
t_redirect	*redirections(t_env *environment, char *line, int empty);
// replace_key_by_value.c
char		*replace_key_by_value(t_env *environment, char *line);
// quotes_interpretation.c
int			quotes_interpretation(t_env *environment, char **line);

				/* One-time actions */

// opening.c
t_env		*opening(int argc, char **argv, char *envp[]);
// closing.c
void		closing_the_program(t_env *environment);
// signal.c
void		default_signal_behavior(void);
void		heredoc_signal_behavior(void);
void		cmd_signal_behavior(void);
// get_environment.c
t_env		*get_environment(char *envp[]);

					/* Environment */

// ftbuiltin_unset.c
void		ftbuiltin_unset_element(t_env *environment, char *arg);
// change_local_variables.c
int			change_local_variables(t_env *environment, char *line, size_t size);
// environment.c
char		**format_environment(t_env *environment);
void		ft_free_environment(t_env *environment);
char		*get_value_by_key(t_env *environment, char *key);
int			env_lstaddback(t_env *env, char *key, char *value, int exported);
// path.c
char		**get_path(char *envp[]);

					/* Pipex */

int		pipex(t_env *environment, char **cmds);
int		first_child(t_env *environment, int pipefd[2], t_cmd **cmds);
int		middle_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr);
void	last_child(t_env *environment, int pipefd[2], t_cmd **cmds, size_t cmdnbr);
// io_open_fds.c
int		io_open_fds(t_redirect *redirect);

// PAS CLASSE
void	ft_free_redirect(t_redirect *redirect);
void	ft_free_cmds_parsed(t_env *environment, t_cmd **tab);

					/* Utils */
void	heredoc_signal_behavior(void);
void	cmd_signal_behavior_parent(void);
void	notatty_signal_behavior(void);
int		use_readline(void);

void	ft_syntaxerror(t_env *environment, char *err);

#endif
