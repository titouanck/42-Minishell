/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _cmd.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:16:57 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CMD_H
# define _CMD_H

# include "minishell_types.h"

int			execute_cmd(t_env *environment, char **args);

void		ftbuiltin_cd(t_env *environment, char **args);

void		ftbuiltin_dma_db(void);

void		ftbuiltin_echo(char **args);

void		ftbuiltin_env(t_env *environment, char **args);

void		ftbuiltin_exit(t_env *environment, char **args, \
			t_cmd **cmds, size_t cmdnbr);

void		ftbuiltin_export(t_env *environment, char **args);

void		ftbuiltin_pwd(char **args);

void		ftbuiltin_unset(t_env *environment, char **args);

#endif
