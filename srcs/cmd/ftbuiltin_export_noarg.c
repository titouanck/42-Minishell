/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftbuiltin_export_noarg.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:14:21 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/24 17:42:42 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_init(t_env *environment)
{
	int	printed;

	printed = environment->printed;
	environment = environment->next;
	while (environment)
	{
		environment->printed = printed;
		environment = environment->next;
	}
}

static void	_print(t_env *closest)
{
	ft_putstr("export ");
	ft_putstr(closest->key);
	if (closest->value)
	{
		ft_putstr("=\"");
		ft_putstr(closest->value);
		ft_putchar('\"');
	}
	ft_putchar('\n');
}

void	export_noarg(t_env *environment)
{
	t_env	*closest;
	t_env	*cur;

	_init(environment);
	while (1)
	{
		closest = NULL;
		cur = environment->next;
		while (cur)
		{
			if (environment->printed == cur->printed && cur->exported \
			&& (closest == NULL || ft_strcmp(cur->key, closest->key) < 0))
				closest = cur;
			cur = cur->next;
		}
		if (closest == NULL)
			break ;
		closest->printed += 1;
		_print(closest);
	}
	environment->printed += 1;
}
