/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:55:26 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 13:52:51 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

# include "minishell_includes.h"

typedef struct s_redirectionlst
{
	char					*str;
	int						redirection_type;
	struct s_redirectionlst	*next;
}							t_redirlst;

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
	int				printed;
	struct s_env	*next;
}					t_env;

typedef struct s_redirect
{
	char			*infile;
	int				fd_infile;
	char			*outfile;
	int				fd_outfile;
	int				append;
	int				to_execute;
	int				last;
	t_redirlst		*lst;
}			t_redirect;

typedef struct s_cmd
{
	char		**args;
	t_redirect	*redirect;
	int			saved_stdout;
	int			saved_stdin;
	pid_t		pid;
}				t_cmd;

#endif