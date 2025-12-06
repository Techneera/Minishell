/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 17:38:25 by rluis-ya@stud    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static void	expand_args(t_data *data);
static void	ft_handle_empty_quote(void);

void	ft_execute_cmd(t_data *data)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	if (data->tree->cmd->args[0][0] == '\"' && \
data->tree->cmd->args[0][1] == '\"' && \
data->tree->cmd->args[0][2] == '\0')
	{
		ft_handle_empty_quote();
		return ;
	}
	expand_args(data);
	if (data->tree->cmd->args[0] && \
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

static void	expand_args(t_data *data)
{
	int		i;
	t_list	*matches;

	i = 0;
	while (data->tree->cmd->args[i])
	{
		data->tree->cmd->args[i] = expand_word(data->tree->cmd->args[i], \
data->envp, ft_exit_status(0, 0, 0));
		if (ft_strchr(data->tree->cmd->args[i], '*'))
		{
			matches = get_wildcard_matches(data->tree->cmd->args[i]);
			if (matches)
			{
				data->tree->cmd->args = insert_wildcard_args(\
data->tree->cmd->args, matches, i);
				i += ft_lstsize(matches);
				ft_lstclear(&matches, free);
				continue ;
			}
		}
		unmask_wildcards(data->tree->cmd->args[i]);
		i++;
	}
	data->tree->cmd->args = ft_realloc_empty(data->tree->cmd->args);
}

static
void	ft_handle_empty_quote(void)
{
	ft_putstr_fd("\"\"", 2);
	ft_putstr_fd(": command not found\n", 2);
	ft_exit_status(127, 1, 0);
}
