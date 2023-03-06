/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _mine.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 14:29:05 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/06 17:36:45 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINE_H
# define _MINE_H

size_t		ft_countc(const char *str, char c);

char		*ft_strchrset(const char *s, char *set);

char		*ft_strrjoin(char const *s1, char const *s2, char const *s3);

void		ft_printtab(char **tab);

void		ft_freetab(char **tab);

int			ft_inset(char c, char *set);

int			ft_strinset(const char *str, const char *set, size_t len);

int			ft_iswhitespace(char c);

char		*ft_strip(char *str);


#endif
