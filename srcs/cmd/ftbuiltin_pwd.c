/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/08 15:23:22 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ftbuiltin_pwd()
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		perror("minishell: pwd");
	else
	{
		ft_putstr(pwd);
		ft_putchar('\n');
		free(pwd);
	}
}
