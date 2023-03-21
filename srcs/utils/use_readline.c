/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_readline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:17:26 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/21 19:17:43 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	use_readline(void)
{
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		return (TRUE);
	else
		return (FALSE);
}
