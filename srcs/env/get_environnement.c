/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_environment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:47:36 by tchevrie          #+#    #+#             */
/*   Updated: 2023/02/23 09:13:17 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_initialisation(void)
{
	t_env	*environment;

	environment = malloc(sizeof(t_env));
	if (!environment)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	environment->key = NULL;
	environment->value = NULL;
	environment->exported = 1;
	environment->next = NULL;
	return (environment);
}

static t_env	*env_nextelement(char *envp[], size_t i, size_t j)
{
	t_env	*element;

	element = malloc(sizeof(t_env));
	if (!element)
		return (ft_putstr_fd(ERRALLOC, 2), NULL);
	element->key = ft_substr(envp[i], 0, j);
	element->value = ft_substr(envp[i], j + 1, ft_strlen(envp[i] + j + 1));
	element->exported = 1;
	element->next = NULL;
	return (element);
}

t_env	*get_environment(char *envp[])
{
	size_t	i;
	size_t	j;
	t_env	*environment;
	t_env	*current_line;

	environment = env_initialisation();
	current_line = environment;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		if (envp[i][j])
		{
			current_line->next = env_nextelement(envp, i, j);
			current_line = current_line->next;
			if (!current_line)
				return (ft_free_environment(environment), NULL);
		}
		i++;
	}
	return (environment);
}
