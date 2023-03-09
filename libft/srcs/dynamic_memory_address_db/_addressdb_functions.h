/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _addressdb_functions.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:29:05 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 17:09:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ADDRESSDB_FUNCTIONS_H
# define _ADDRESSDB_FUNCTIONS_H

typedef struct s_address
{
	void				*address;
	struct s_address	*next;
}						t_address;

int		addressdb_lstinsert(t_address *db, void *address);

int		addressdb_lstdel(t_address *db_elem, void *address, \
		void (*del)(void *));

int		addressdb_lstcheck(t_address *db_elem, void *address);

int		addressdb_lstprint(t_address *db);

void	addressdb_lsterase(t_address **db, void (*del)(void *));

#endif
