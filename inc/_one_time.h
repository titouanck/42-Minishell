/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _one_time.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:16:57 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ONE_TIME_H
# define _ONE_TIME_H

# include "minishell_types.h"

void		closing_the_program(t_env *environment);

void		init_logs(void);

t_env		*opening(int argc, char **argv, char *envp[]);

void		write_to_logfile(int fd, t_log log, char *last_input);

#endif
