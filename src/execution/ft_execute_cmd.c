#include "execution.h"

void	ft_execute_cmd(t_data *data)
{
	pid_t	pid;
	char	**args;
	int		i;

	if (!data->tree)
		return ;
	i = 0;
	args = data->tree->cmd->args;
	while (args[i])
	{
		args[i] = expand_word(args[i], data->envp, data->status);
		i++;
	}
	if (!is_builtin(data, data->tree->cmd->args[0]))
	{
		if (!init_pid(&pid, &data->fds))
			secure_exit(data, FAIL_STATUS);
		if (pid == 0)
			ft_child_cmd(data, data->envp);
	}
	update_positions(data);
}
