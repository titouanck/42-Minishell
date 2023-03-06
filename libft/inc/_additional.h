/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _additional.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:26:17 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/06 16:35:26 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ADDITIONAL_H
# define _ADDITIONAL_H

# include <unistd.h>

char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*ft_strjoin(char const *s1, char const *s2);

char	*ft_strtrim(char const *s1, char const *set);

char	**ft_split(char const *s, char c);

char	*ft_itoa(int n);

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

void	ft_putchar_fd(char c, int fd);

void	ft_putstr_fd(char const *s, int fd);

void	ft_putendl_fd(char const *s, int fd);

void	ft_putnbr_fd(int n, int fd);

void	ft_striteri(char *s, void (*f)(unsigned int, char *));

#endif
