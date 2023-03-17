/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:24:56 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/16 19:37:04 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_lstprint(t_redirectionlst *lst)
{
	if (lst && lst->str == NULL)
		lst = lst->next;
	while (lst)
	{
		ft_printf("[%s, \'%c\']", lst->str, lst->redirection_type);
		if (lst->next)
			ft_printf(" → ");
		else
			ft_printf("\n");
		lst = lst->next;
	}
}

t_redirectionlst	*redirection_lstdel(t_redirectionlst *lst_elem, char *str)
{
	t_redirectionlst	*lst_prev;
	t_redirectionlst	*lst_next;

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

void	redirection_lstclear(t_redirectionlst *lst)
{
	t_redirectionlst	*lst_elem;
	t_redirectionlst	*next;

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


t_redirectionlst	*redirection_lstaddback(t_redirectionlst *lst, char *str, int redirection_type)
{
	t_redirectionlst	*new;
	t_redirectionlst	*current;

	new = db_malloc(sizeof(t_redirectionlst));
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
