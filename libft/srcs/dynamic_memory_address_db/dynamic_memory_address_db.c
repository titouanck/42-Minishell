/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_memory_address_db.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:54:03 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 14:14:22 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_addressdb_functions.h"
#include "libft.h"

static int	addressdb_lstsize(t_address *db)
{
	int	n;

	if (!db)
		return (0);
	n = 0;
	while (db->next)
	{
		db = db->next;
		n++;
	}
	return (n);
}

int	dynamic_memory_address_db(unsigned char action, void *address)
{
	static t_address	*db;

	if (!db)
	{
		db = malloc(sizeof(t_address));
		if (!db)
			return (ft_putstr_fd("Impossible initialisation of " \
			"function dynamic_memory_address_db(int, void *)\n", 2), 0);
		db->address = NULL;
		db->next = NULL;
	}
	if (action == ADDRESSDB_INSERTION && address)
		return (addressdb_lstinsert(db, address));
	else if (action == ADDRESSDB_DELETION && address)
		return (addressdb_lstdel(db, address, free));
	else if (action == ADDRESSDB_CHECK && address)
		return (addressdb_lstcheck(db, address));
	else if (action == ADDRESSDB_PRINT)
		return (addressdb_lstprint(db, (int *) address));
	else if (action == ADDRESSDB_SIZE)
		return (addressdb_lstsize(db));
	else if (action == ADDRESSDB_ERASURE)
		return (addressdb_lsterase(&db, free), !ft_isnotnull(db));
	return (0);
}
