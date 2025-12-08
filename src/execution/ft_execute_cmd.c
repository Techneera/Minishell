/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/07 21:48:57 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static
void	ft_handle_empty_quote()
{
	ft_putstr_fd("\"\"", 2);
	ft_putstr_fd(": command not found\n", 2);
	ft_exit_status(127, 1, 0);
}

void	ft_execute_cmd(t_data *data)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	if (data->tree->cmd->args[0] && \
data->tree->cmd->args[0][0] == '\"' && \
data->tree->cmd->args[0][1] == '\"' && \
data->tree->cmd->args[0][2] == '\0')
		return (ft_handle_empty_quote());
	expand_args(data);
	if (data->tree->cmd->args && data->tree->cmd->args[0] && \
!ft_is_builtin(data, data->tree->cmd->args[0]))
	{
		if (!init_pid(&pid, &data->fds))
			secure_exit(data, FAIL_STATUS);
		if (pid == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			ft_child_cmd(data, data->envp);
		}
	}
	else
		data->fds->pos.fork_id++;
	update_positions(data);
}
