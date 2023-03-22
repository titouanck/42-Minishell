/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_log_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchevrie <tchevrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:01:06 by tchevrie          #+#    #+#             */
/*   Updated: 2023/03/22 16:02:31 by tchevrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_log_files(t_env *environment)
{
	db_free(environment->log.infile);
	environment->log.infile = NULL;
	db_free(environment->log.outfile);
	environment->log.outfile = NULL;
}
