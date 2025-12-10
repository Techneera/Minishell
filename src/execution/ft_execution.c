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

int	ft_execution(t_data *data)
{
	int	i;
	int	child_status;

	if (!data->env_list)
		return (0);
	child_status = 0;
	if (!data->tree)
		return (-1);
	ft_create_fds(data);
	ft_exec_tree(data, data->envp);
	ft_closing_all(&data->fds);
	i = -1;
	while (data->fds && ++i < data->fds->get.n_cmds && data->fds->c_pids)
	{
		signal(SIGINT, &handle_sigint_wait);
		if (waitpid(data->fds->c_pids[i], &child_status, 0) > 0)
		{
			if (WIFEXITED(child_status))
				ft_exit_status(WEXITSTATUS(child_status), 1, 0);
			else if (WIFSIGNALED(child_status))
				ft_exit_status(WTERMSIG(child_status) + 128, 1, 0);
		}
	}
	free_data(data);
	return (0);
}
