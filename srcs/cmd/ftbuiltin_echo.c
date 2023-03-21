/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 18:15:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/20 13:31:38 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_no_newline_option(char **args, size_t *i_ptr, int *j_ptr, int *newline_ptr)
{
	*j_ptr = 2;
	while (args[*i_ptr][*j_ptr])
	{
		if (args[*i_ptr][*j_ptr] != 'n')
		{
			*j_ptr = -1;
			break ;
		}
		*j_ptr += 1;
	}
	if (*j_ptr > 0)
	{
		*newline_ptr = FALSE;
		*i_ptr += 1;
		return (1);
	}
	else
		return (0);
}

void	ftbuiltin_echo(char **args)
{
	size_t	i;
	int		j;
	int		newline;

	g_returnval = 0;
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	newline = TRUE;
	i = 1;
	while (args[i] && (ft_strncmp(args[i], "-n", 2) == 0))
		if (!_no_newline_option(args, &i, &j, &newline))
			break ;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
}
