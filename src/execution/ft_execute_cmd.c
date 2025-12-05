#include "execution.h"

static void	expand_args(t_data *data);

void	ft_execute_cmd(t_data *data)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	expand_args(data);
	if (!ft_is_builtin(data, data->tree->cmd->args[0]))
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
