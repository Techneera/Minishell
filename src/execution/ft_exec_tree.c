/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

int	ft_exec_tree(t_data	*data, char **envp)
{
	t_fds	*fds;
	t_ast	*node;

	fds = data->fds;
	node = data->tree;
	if (node == NULL || !fds)
		return (0);
	if (node->type == NODE_CMD)
		ft_execute_cmd(data);
	else if (node->type == NODE_PIPE)
		ft_execute_pipe(data);
	else if (node->type == NODE_SUBSHELL)
		ft_execute_sshell(data, envp);
	else if (node->type == NODE_AND)
		execute_and(data, envp);
	else if (node->type == NODE_OR)
		execute_or(data, envp);
	return (0);
}
