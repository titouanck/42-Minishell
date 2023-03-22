/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddress_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 16:41:38 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:58:18 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_ft_printf.h"

static size_t	printaddress(const unsigned long n, int fd)
{
	if (n / 16)
		return (printaddress(n / 16, fd) + printaddress(n % 16, fd));
	else if (!(n / 10))
		ft_putchar_fd(n + '0', fd);
	else
		ft_putchar_fd((char) n - 10 + 'a', fd);
	return (1);
}

size_t	ft_putaddress_fd(void *address, int fd)
{
	if (!address)
		return (ft_putstr_fd("(nil)", fd), 5);
	ft_putstr_fd("0x", fd);
	return (2 + printaddress((unsigned long) address, fd));
}
