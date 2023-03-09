/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_memory_address_db.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:54:03 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/09 13:55:41 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_addressdb_functions.h"
#include "libft.h"

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
	else if (action == ADDRESSDB_PRINT && !address)
		return (addressdb_lstprint(db));
	else if (action == ADDRESSDB_ERASURE && !address)
		return (addressdb_lsterase(&db, free), !ft_isnotnull(db));
	return (0);
}
