/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_check_syntax.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:13:30 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/27 15:41:43 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_ambiguous(t_env *env, char *str, t_redirect *redirect)
{
	size_t	i;
	char	*tmp;

	if (str[0] && str[1] && str[2] && ft_isdigit(str[1]))
		return (ft_memmove(str, str + 2, ft_strlen(str + 2) + 1), \
		redirection_check_syntax(env, str, redirect));
	else
	{
		i = 1;
		while (str[i] && ft_inset(str[i], VARNAMESET))
			i++;
		tmp = ft_strdup(str);
		ft_memmove(str, str + i, ft_strlen(str + i) + 1);
		if (ft_strlen(str) > 0 && *str != SEPARATOR \
		&& *str != LEFTCHEVRON && *str != RIGHTCHEVRON)
			return (free(tmp), redirection_check_syntax(env, str, redirect));
		g_returnval = 1;
		tmp[i] = '\0';
		minishell_error("$", tmp + 1);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("ambiguous redirect\n", 2);
		redirect->to_execute = FALSE;
		return (free(tmp), -1);
	}
}

int	redirection_check_syntax(t_env *environment, char *str, \
	t_redirect *redirect)
{
	if (*str == LEFTCHEVRON)
		ft_syntaxerror(environment, "<");
	else if (*str == RIGHTCHEVRON)
		ft_syntaxerror(environment, ">");
	else if (*str == EMPTYVARIABLE)
		return (_ambiguous(environment, str, redirect));
	else if (!*str && redirect->last)
		ft_syntaxerror(environment, "newline");
	else if (!*str && !redirect->last)
		ft_syntaxerror(environment, "|");
	else
		return (1);
	redirection_lstclear(redirect->lst);
	redirect->lst = NULL;
	return (0);
}
