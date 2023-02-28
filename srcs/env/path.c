

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 01:02:28 by tchevrie          #+#    #+#             */
/*   Updated: 2023/01/25 17:39:29 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path_ptr(char *envp[])
{
	int		i;
	char	*ptr;

	i = 0;
	while (1)
	{
		ptr = envp[i];
		if (!ptr)
			break ;
		if (strncmp(ptr, "PATH=", 5) == 0)
			return (ptr);
		i++;
	}
	return (NULL);
}

char	**get_path(char *envp[])
{
	char	*path_ptr;
	char	**path;

	if (!envp)
		return (NULL);
	path_ptr = get_path_ptr(envp);
	if (!path_ptr)
		return (NULL);
	path_ptr += 5;
	path = ft_split(path_ptr, ':');
	return (path);
}
