#include "execution.h"

void	ft_execute_sshell(t_data *data, char **envp)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	if (!init_pid(&pid, &data->fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
	{
		apply_std_dup(data);
		apply_redirs_subshell(data);
		data->tree = data->tree->body;
		ft_exec_tree(data, envp);
		secure_exit(data, 42);
	}
	update_positions(data);
}
