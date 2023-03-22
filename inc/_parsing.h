/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _parsing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:16:57 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PARSING_H
# define _PARSING_H

# include "minishell_types.h"

t_cmd		*parse_args(t_env *environment, char **line, int last);

int			parse_builtin(t_env *environment, char **args, \
			t_cmd **cmds, size_t cmdnbr);

t_cmd		*parse_cmd(t_env *environment, char **line, int last);

void		parse_heredoc_limiter(t_env *environment, char *line);

void		parsing(t_env *environment, char **line);

int			quotes_interpretation(t_env *environment, char **line);

char		**split_cmds(t_env *environment, char **ptr);

#endif
