/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _utils.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:21:02 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _UTILS_H
# define _UTILS_H

# include "minishell_types.h"

int			event(void);

void		exit_erralloc(t_env *environment);

void		free_log_files(t_env *environment);

void		ft_syntaxerror(t_env *environment, char *err);

void		minishell_error(const char *s1, const char *s2);

void		remove_quote_token_line(char *line);

t_env		*saved_environment(t_env *environment);

int			use_readline(void);

#endif
