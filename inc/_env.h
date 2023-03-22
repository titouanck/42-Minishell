/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _env.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:16:57 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ENV_H
# define _ENV_H

# include "minishell_types.h"

int			change_local_variables(t_env *environment, char *line, size_t size);

int			env_lstaddback(t_env *env, char *key, char *value, int exported);

char		**format_environment(t_env *environment);

void		ft_free_environment(t_env *environment);

t_env		*get_environment(char *envp[]);

char		*get_value_by_key(t_env *environment, char *key);

char		**get_path(char *envp[]);

char		*replace_key_by_value(char *line);

void		update_environment(t_env *environment, \
			char *key, char *value, int append);

void		update_local_variables(t_env *environment, \
			char *key, char *value, int append);

#endif
