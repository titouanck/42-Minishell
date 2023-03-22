/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:59:35 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 13:59:48 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_redirect(t_redirect *redirect)
{
	if (!redirect)
		return ;
	if (redirect->lst)
		redirection_lstclear(redirect->lst);
	db_free(redirect->infile);
	db_free(redirect->outfile);
	db_free(redirect);
}
