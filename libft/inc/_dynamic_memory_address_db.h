/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _dynamic_memory_address_db.h                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:26:17 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/14 14:59:16 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DYNAMIC_MEMORY_ADDRESS_DB_H
# define _DYNAMIC_MEMORY_ADDRESS_DB_H

# include "libft.h"

# define ADDRESSDB_INSERTION '+'
# define ADDRESSDB_DELETION '-'
# define ADDRESSDB_CHECK 'C'
# define ADDRESSDB_PRINT 'P'
# define ADDRESSDB_ERASURE 'E'
# define ADDRESSDB_SIZE 'S'

int		dynamic_memory_address_db(unsigned char action, void *address);

void	*db_malloc(size_t bytes);

void	*db_calloc(size_t count, size_t size);

void	*db_strdup(void *old_address);

void	*db_strndup(void *old_address, size_t n);

void	db_free(void *address);

void	db_freetab(char **tab);

char	**db_split(char const *s, char c);

char	*db_substr(char const *s, unsigned int start, size_t len);

char	*db_strjoin(char const *s1, char const *s2);

char	*db_strrjoin(char const *s1, char const *s2, char const *s3);

#endif
