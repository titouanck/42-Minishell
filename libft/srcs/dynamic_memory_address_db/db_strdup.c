/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   db_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:29:05 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 17:09:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*db_strdup(void *old_address)
{
	void	*new_address;

	new_address = ft_strdup(old_address);
	if (new_address)
		dynamic_memory_address_db('+', new_address);
	return (new_address);
}
