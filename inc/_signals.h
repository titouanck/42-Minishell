/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _signals.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:20:11 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SIGNALS_H
# define _SIGNALS_H

# include "minishell_types.h"

void		default_signal_tty(void);

void		default_signal_notty(void);

void		cmd_signal_child(void);

void		cmd_signal_parent(void);

void		heredoc_signal(void);

#endif
