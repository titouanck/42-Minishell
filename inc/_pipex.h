/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _pipex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:23:22 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PIPEX_H
# define _PIPEX_H

# include "minishell_types.h"

int			action_on_files(t_redirect *redirect, \
			t_redirlst *current, int heredoc);

void		error_on_open(t_redirect *redirect, char *file);

void		exit_child(t_env *environment, t_cmd **cmds, size_t cmdnbr);

int			first_child(t_env *environment, int pipefd[2], t_cmd **cmds);

void		ft_free_cmds_parsed(t_cmd **tab);

t_cmd		**get_cmds_parsed(t_env *environment, char **cmds);

int			io_open_fds(t_env *environment, t_redirect *redirect);

void		last_child(t_env *environment, int pipefd[2], \
			t_cmd **cmds, size_t cmdnbr);

int			middle_child(t_env *environment, int pipefd[2], \
			t_cmd **cmds, size_t cmdnbr);

int			open_fds(t_env *environment, t_cmd **cmds, size_t cmdnbr);

int			pipex(t_env *environment, char **cmds);

#endif
