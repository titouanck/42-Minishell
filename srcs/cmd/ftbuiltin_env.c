/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:25:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:29:50 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ftbuiltin_env()
{
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
	environment->g_returnval = 0;
}
