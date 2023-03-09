/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   db_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:29:05 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 17:09:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*db_calloc(size_t count, size_t size)
{
	void	*address;

	address = ft_calloc(count, size);
	if (address)
		dynamic_memory_address_db('+', address);
	return (address);
}
