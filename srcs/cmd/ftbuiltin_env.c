/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:25:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 13:17:31 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_error_handling(char **args)
{
	if (args && args[0] && args[1] && ft_strncmp(args[1], "-", 1) == 0)
	{
		if (ft_strncmp(args[1], "--", 2) == 0)
		{
			ft_putstr_fd("env: unrecognized option \'", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd("\'\n", 2);
		}
		else if (ft_strncmp(args[1], "-", 1) == 0)
		{
			if (ft_strlen(args[1]) >= 2)
				args[1][2] = '\0';
			ft_putstr_fd("env: invalid option -- \'", 2);
			ft_putstr_fd(args[1] + 1, 2);
			ft_putstr_fd("\'\n", 2);
		}
		g_returnval = 125;
		return (0);
	}
	else
		return (1);
}

void	ftbuiltin_env(t_env *environment, char **args)
{
	g_returnval = 0;
	if (!_error_handling(args))
		return ;
	while (environment)
	{
		if (environment->key && environment->exported)
		{
			write(1, environment->key, ft_strlen(environment->key));
			write(1, "=", 1);
			write(1, environment->value, ft_strlen(environment->value));
			write(1, "\n", 1);
		}	
		environment = environment->next;
	}
}
