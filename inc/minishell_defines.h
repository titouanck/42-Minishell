/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_defines.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 17:59:22 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 17:52:14 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEFINES_H
# define MINISHELL_DEFINES_H

# define PROMPT "minishell →"
# define RL_NEWLINE "\001"
# define ERRALLOC "minishell: Could not allocate memory.\n"
# define INVALIDOPTION 2
# define VARNAMESET "abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789_"

# define PIPECHAR -1
# define VARKEY -2
# define SEPARATOR -3
# define LEFTCHEVRON -4
# define RIGHTCHEVRON -5
# define HEREDOC -6
# define NOTAVARKEY -7
# define QUOTES -8
# define EMPTYVARIABLE -9

# define INFILE_FILE 'r'
# define INFILE_HEREDOC 'h'
# define OUTFILE_TRUNC 'w'
# define OUTFILE_APPEND 'a'

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

#endif