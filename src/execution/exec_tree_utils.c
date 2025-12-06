/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

int	init_pid(pid_t *pid, t_fds **fds)
{
	*pid = fork();
	if (*pid != 0)
	{
		(*fds)->c_pids[(*fds)->pos.fork_id] = *pid;
		(*fds)->pos.fork_id++;
	}
	if (*pid == -1)
	{
		perror("fork");
		return (0);
	}
	return (1);
}
