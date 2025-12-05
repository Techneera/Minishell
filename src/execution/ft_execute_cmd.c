#include "execution.h"

void	ft_execute_cmd(t_data *data)
{
	pid_t	pid;
	int		i;
	t_list	*matches;

	if (!data->tree)
		return ;
	i = 0;
	while (data->tree->cmd->args[i])
	{
		data->tree->cmd->args[i] = expand_word(data->tree->cmd->args[i], data->envp, ft_exit_status(0, 0, 0));
		if (ft_strchr(data->tree->cmd->args[i], '*'))
		{
			matches = get_wildcard_matches(data->tree->cmd->args[i]);
			if (matches)
			{
				data->tree->cmd->args = insert_wildcard_args(data->tree->cmd->args, matches, i);
				i += ft_lstsize(matches);
				ft_lstclear(&matches, free);
				continue ;
			}
		}
		unmask_wildcards(data->tree->cmd->args[i]);
		i++;
	}
	data->tree->cmd->args = ft_realloc_empty(data->tree->cmd->args);
	if (!ft_is_builtin(data, data->tree->cmd->args[0]))
	{
		if (!init_pid(&pid, &data->fds))
			secure_exit(data, FAIL_STATUS);
		if (pid == 0)
			ft_child_cmd(data, data->envp);
	}
	else
		data->fds->pos.fork_id++;
	update_positions(data);
}
