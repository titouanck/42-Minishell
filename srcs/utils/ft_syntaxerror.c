/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntaxerror.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:15:42 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/13 11:56:01 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_syntaxerror(t_env *environment, char *err)
{
	g_returnval = 2;

	if (err)
	{
		ft_putstr_fd("minishell: syntax error near unexpected `", 2);
		ft_putstr_fd(err, 2);
		ft_putstr_fd("\'\n", 2);
	}
	if (!use_readline())
	{
		if (environment)
		{
			ft_putstr_fd("minishell: line", 2);
			ft_putnbr_fd(environment->line_nbr, 2);
			ft_putstr_fd(": `", 2);
			ft_putstr_fd(environment->last_input, 2);
			ft_putstr_fd("\'\n", 2);
		}
		closing_the_program(NULL);
		exit(g_returnval);
	}
}
