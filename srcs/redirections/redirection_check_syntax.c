/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_check_syntax.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:13:30 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 14:13:45 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirection_check_syntax(t_env *environment, char c, t_redirect *redirect)
{
	if (c == LEFTCHEVRON)
		ft_syntaxerror(environment, "<");
	else if (c == RIGHTCHEVRON)
		ft_syntaxerror(environment, ">");
	else if (!c && redirect->last)
		ft_syntaxerror(environment, "newline");
	else if (!c && !redirect->last)
		ft_syntaxerror(environment, "|");
	else
		return (1);
	redirection_lstclear(redirect->lst);
	redirect->lst = NULL;
	return (0);
}
