/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:49:33 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 18:48:06 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ftbuiltin_pwd(char **args)
{
	char	*pwd;

	if (args && args[0] && args[1] && \
	ft_strlen(args[1]) >= 2 && ft_strncmp(args[1], "-", 1) == 0)
	{
		args[1][2] = '\0';
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_returnval = INVALIDOPTION;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		perror("minishell: pwd");
	else
	{
		ft_putstr(pwd);
		ft_putchar('\n');
		db_free(pwd);
	}
}
