/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_includes.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:57:17 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 18:16:57 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_INCLUDES_H
# define MINISHELL_INCLUDES_H

# include "minishell_types.h"
# include "minishell_defines.h"

# include "libft.h"

# include <readline/readline.h>
# include <readline/history.h>

# include <stdio.h>

# include <signal.h>

# include <errno.h>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

#endif