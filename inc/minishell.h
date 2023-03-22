/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:17:29 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 16:50:07 by tchevrie         ###   ########.fr       */
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
# define RL_NEWLINE "\001"
# define ERRALLOC "minishell: Could not allocate memory.\n"
# define INVALIDOPTION 2
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

# define INFILE_FILE 'r'
# define INFILE_HEREDOC 'h'
# define OUTFILE_TRUNC 'w'
# define OUTFILE_APPEND 'a'

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

extern int	g_returnval;

typedef struct s_redirectionlst
{
	char					*str;
	int						redirection_type;
	struct s_redirectionlst	*next;
}							t_redirectionlst;

typedef struct s_log
{
	char	**args;
	char	*infile;
	char	*outfile;
}			t_log;

typedef struct s_env
{
	char			*prompt;
	t_log			log;
	char			*line;
	size_t			line_nbr;
	char			*last_input;
	char			*key;
	char			*value;
	int				exported;
	int				limiter_between_quotes;
	char			*heredoc_files;
	struct s_env	*next;
}					t_env;

typedef struct	s_redirect
{
	char		*infile;
	int			fd_infile;
	char		*outfile;
	int			fd_outfile;
	int			append;
	int			to_execute;
	int			last;
	t_redirectionlst		*lst;
}			t_redirect;

typedef struct s_cmd
{
	char		**args;
	t_redirect	*redirect;
	int			saved_stdout;
	int			saved_stdin;
	pid_t		pid;
}				t_cmd;

					/* Parsing */

// parse_builtin.c
int	parse_builtin(t_env *environment, char **args, \
	t_cmd **cmds, size_t cmdnbr);
// split_cmds.c
char		**split_cmds(t_env *environment, char **ptr);
// parsing.c
void		parsing(t_env *environment, char **line);
// parse_cmd.c
t_cmd		*parse_cmd(t_env *environment, char **line, int last);
// parse_args.c
t_cmd		*parse_args(t_env *environment, char **line, int last);
// redirections.c
t_redirect	*redirections(t_env *environment, char *line, int last);
// quotes_interpretation.c
int			quotes_interpretation(t_env *environment, char **line);

				/* One-time actions */

// opening.c
t_env		*opening(int argc, char **argv, char *envp[]);
// closing.c
void		closing_the_program(t_env *environment);
// signal.c
void		default_signal_tty(void);
void		heredoc_signal(void);
void		cmd_signal_child(void);
// get_environment.c
t_env		*get_environment(char *envp[]);

					/* Environment */

// ftbuiltin_unset.c
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
int	io_open_fds(t_env *environment, t_redirect *redirect);

					/* Utils */
void	heredoc_signal(void);
void	cmd_signal_parent(void);
void	default_signal_notty(void);

void	ft_syntaxerror(t_env *environment, char *err);

// void	remove_quote_token_line(char *line);
t_redirectionlst	*redirection_lstaddback(t_redirectionlst *lst, char *str, int redirection_type);
t_redirectionlst	*redirection_lstdel(t_redirectionlst *lst_elem, char *str);
void		redirection_lstprint(t_redirectionlst *lst);
void	redirection_lstclear(t_redirectionlst *lst);

void	exit_erralloc(t_env *environment);


// NEW

	/* routine */
void	rm_heredoc_files(t_env *environment);
int		use_argv(t_env *environment, int argc, char **argv);
char	*new_prompt(t_env *environment);

	/* one-time */
void	init_logs(void);
void	write_to_logfile(int fd, t_log log, char *last_input);

	/* utils */
int		event(void);
void	exit_erralloc(t_env *environment);
void	ft_syntaxerror(t_env *environment, char *err);
int		use_readline(void);
void	minishell_error(const char *s1, const char *s2);
t_env	*saved_environment(t_env *environment);
void	remove_quote_token_line(char *line);
void	free_log_files(t_env *environment);

	/* env */

void	update_environment(t_env *environment, \
		char *key, char *value, int append);
void	update_local_variables(t_env *environment, \
	char *key, char *value, int append);
char	*replace_key_by_value(char *line);
	/* cmd */

void		ftbuiltin_echo(char **args);
void		ftbuiltin_cd(t_env *environment, char **args);
void		ftbuiltin_pwd(char **args);
void		ftbuiltin_export(t_env *environment, char **args);
void		ftbuiltin_unset(t_env *environment, char **args);
void		ftbuiltin_env(t_env *environment, char **args);
void		ftbuiltin_exit(t_env *environment, char **args, t_cmd **cmds, size_t cmdnbr);
void		ftbuiltin_dma_db(void);
int			execute_cmd(t_env *environment, char **args);

	/* parsing */
void	parse_heredoc_limiter(t_env *environment, char *line);

	/* redirections */
void	ft_free_redirect(t_redirect *redirect);
int		open_heredoc(t_env *environment, t_redirect *redirect);
int		new_redirection(t_env *environment, char *line, \
		t_redirect *redirect, int redirection_type);
int		redirection_check_syntax(t_env *environment, char c, t_redirect *redirect);

	/* pipex */
void	ft_free_cmds_parsed(t_cmd **tab);
t_cmd	**get_cmds_parsed(t_env *environment, char **cmds);
int		action_on_files(t_redirect *redirect, \
		t_redirectionlst *current, int heredoc);
void	error_on_open(t_redirect *redirect, char *file);
int		open_fds(t_env *environment, t_cmd **cmds, size_t cmdnbr);
void	exit_child(t_env *environment, t_cmd **cmds, size_t cmdnbr);

#endif
