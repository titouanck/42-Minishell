/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddress.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 16:41:38 by tchevrie          #+#    #+#             */
/*   Updated: 2022/11/08 10:22:22 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_ft_printf.h"

static size_t	printaddress(const unsigned long n)
{
	if (n / 16)
		return (printaddress(n / 16) + printaddress(n % 16));
	else if (!(n / 10))
		ft_putchar(n + '0');
	else
		ft_putchar((char) n - 10 + 'a');
	return (1);
}

size_t	ft_putaddress(void *address)
{
	if (!address)
		return (ft_putstr("(nil)"));
	ft_putstr("0x");
	return (2 + printaddress((unsigned long) address));
}
