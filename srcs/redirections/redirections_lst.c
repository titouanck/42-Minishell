/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/27 15:41:14 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirlst	*redirection_lstdel(t_redirlst *lst_elem, char *str)
{
	t_redirlst	*lst_prev;
	t_redirlst	*lst_next;

	if (!lst_elem)
		return (NULL);
	while (lst_elem->str != str && lst_elem->next)
	{
		lst_prev = lst_elem;
		lst_elem = lst_elem->next;
	}
	if (lst_elem->str == str)
	{
		lst_next = lst_elem->next;
		db_free(lst_elem->str);
		db_free(lst_elem);
		lst_prev->next = lst_next;
		return (lst_prev);
	}
	return (NULL);
}

void	redirection_lstclear(t_redirlst *lst)
{
	t_redirlst	*lst_elem;
	t_redirlst	*next;

	if (!lst)
		return ;
	lst_elem = lst;
	while (lst_elem)
	{
		next = lst_elem->next;
		db_free(lst_elem->str);
		db_free(lst_elem);
		lst_elem = next;
	}
}

void	redirection_rmlast(t_redirect *redirect)
{
	t_redirlst	*lst_elem;
	t_redirlst	*lst_prev;

	if (!redirect || !redirect->lst)
		return ;
	if (!redirect->lst->next)
	{
		db_free(redirect->lst->str);
		db_free(redirect->lst);
		redirect->lst = NULL;
		return ;
	}	
	lst_elem = redirect->lst;
	while (lst_elem->next)
	{
		lst_prev = lst_elem;
		lst_elem = lst_elem->next;
	}
	db_free(lst_elem->str);
	db_free(lst_elem);
	lst_prev->next = NULL;
}

t_redirlst	*redirection_lstaddback(t_redirlst *lst, char *str, \
	int redirection_type)
{
	t_redirlst	*new;
	t_redirlst	*current;

	new = db_malloc(sizeof(t_redirlst));
	if (!new)
		return (lst);
	new->str = str;
	new->redirection_type = redirection_type;
	new->next = NULL;
	if (lst)
	{
		current = lst;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	else
		lst = new;
	return (lst);
}
