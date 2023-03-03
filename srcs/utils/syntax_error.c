/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 18:16:28 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/02 18:18:30 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error_near(char c)
{
	char	token[2];
	char	*errmsg;

	token[0] = c;
	token[1] = '\0';
	errmsg = ft_strrjoin("minishell: syntax error near unexpected `", token, "\'\n");
	if (!errmsg)
		return ;
	ft_putstr_fd(errmsg, 2);
	free(errmsg);
}
