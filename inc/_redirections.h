/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _redirections.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:22:25 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REDIRECTIONS_H
# define _REDIRECTIONS_H

# include "minishell_types.h"

void		ft_free_redirect(t_redirect *redirect);

int			open_heredoc(t_env *environment, t_redirect *redirect);

int			new_redirection(t_env *environment, char *line, \
			t_redirect *redirect, int redirection_type);

int			redirection_check_syntax(t_env *environment, char c, \
			t_redirect *redirect);

t_redirlst	*redirection_lstaddback(t_redirlst *lst, char *str, \
			int redirection_type);

t_redirlst	*redirection_lstdel(t_redirlst *lst_elem, char *str);

void		redirection_lstclear(t_redirlst *lst);

t_redirect	*redirections(t_env *environment, char *line, int last);

#endif
