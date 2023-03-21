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

	environment = db_malloc(sizeof(t_env));
	if (!environment)
		exit_erralloc(NULL);
	environment->key = NULL;
	environment->value = NULL;
	environment->exported = 1;
	environment->next = NULL;
	return (environment);
}

static t_env	*env_nextelement(char *envp[], size_t i, size_t j)
{
	t_env	*element;

	element = db_malloc(sizeof(t_env));
	if (!element)
		exit_erralloc(NULL);
	element->key = db_substr(envp[i], 0, j);
	element->value = db_substr(envp[i], j + 1, ft_strlen(envp[i] + j + 1));
	element->exported = 1;
	element->next = NULL;
	return (element);
}

static void	increment_shlvl(t_env *environment)
{
	int	shlvl;

	while (1)
	{
		if (!environment)
			break ;
		if (ft_strcmp(environment->key, "SHLVL") == 0)
		{
			shlvl = ft_atoi(environment->value);
			db_free(environment->value);
			shlvl += 1;
			environment->value = ft_itoa(shlvl);
			break ;
		}
		environment = environment->next;
	}
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
	increment_shlvl(environment);
	return (environment);
}
