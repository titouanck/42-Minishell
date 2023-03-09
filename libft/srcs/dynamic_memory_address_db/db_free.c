/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   db_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:29:05 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 17:09:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	db_free(void *address)
{
	if (address)
	{
		free(address);
		dynamic_memory_address_db('-', address);
	}
}
