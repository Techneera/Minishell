#include "execution.h"

void	ft_execute_cmd(t_data *data, char **envp)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	if (!init_pid(&pid, &data->fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
		ft_child_cmd(data, envp);
	update_positions(data);
}
