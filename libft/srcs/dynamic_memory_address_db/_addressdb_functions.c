/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _addressdb_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:29:05 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 17:09:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_addressdb_functions.h"
#include "libft.h"

int	addressdb_lstinsert(t_address *db, void *address)
{
	t_address	*new;

	if (!db)
		return (0);
	while (db->next)
		db = db->next;
	new = malloc(sizeof(t_address));
	if (!new)
		return (0);
	new->address = address;
	new->next = NULL;
	db->next = new;
	return (1);
}

int	addressdb_lstdel(t_address *db_elem, void *address, void (*del)(void *))
{
	t_address	*db_prev;
	t_address	*db_next;

	if (!db_elem)
		return (0);
	while (db_elem->address != address && db_elem->next)
	{
		db_prev = db_elem;
		db_elem = db_elem->next;
	}
	if (db_elem->address == address)
	{
		db_next = db_elem->next;
		del(db_elem);
		db_prev->next = db_next;
		return (1);
	}
	return (0);
}

int	addressdb_lstcheck(t_address *db_elem, void *address)
{
	if (!db_elem)
		return (0);
	while (db_elem->address != address && db_elem->next)
		db_elem = db_elem->next;
	if (db_elem->address == address)
		return (1);
	return (0);
}

int	addressdb_lstprint(t_address *db)
{
	while (db)
	{
		if (db->address)
			ft_printf("%p\n", db->address);
		db = db->next;
	}
	return (1);
}

void	addressdb_lsterase(t_address **db, void (*del)(void *))
{
	if (!db || !(*db) || !del)
		return ;
	if ((*db)->next)
		addressdb_lsterase((&(*db)->next), del);
	del((*db)->address);
	del(*db);
	*db = NULL;
}
