/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   saved_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:31:52 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 12:32:51 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*saved_environment(t_env *environment)
{
	static t_env	*saved_env;

	if (environment)
		saved_env = environment;
	return (saved_env);
}