#include "execution.h"

int child_status = 0;

int	ft_execution(t_data *data)
{
	int	g_signal;
	int	i;

	if (!data->env_list)
		return (0);
	g_signal = 0;
	if (!data->tree)
		return (-1);
	ft_create_fds(data);
	ft_exec_tree(data, data->envp);
	ft_closing_all(&data->fds);
	i = 0;
	while(data->fds && i < data->fds->get.n_cmds && data->fds->c_pids && waitpid(data->fds->c_pids[i], &child_status, 0) > 0)
	{
		if (WIFEXITED(child_status))
			g_signal = WEXITSTATUS(child_status);
		i++;
	}
	free_fds(&data->fds);
	free_tree(&data->root);
	return(g_signal);
}
