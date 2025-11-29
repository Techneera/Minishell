#include "execution.h"

int	wait_bonus(t_data *data, t_fds *fds)
{
	int	signal;
	int	i;

	signal = 0;
	i = 0;
	while(i < fds->get.n_cmds && fds->c_pids && waitpid(fds->c_pids[i], &child_status, 0) > 0)
	{
		if (WIFEXITED(child_status))
		{
			signal = WEXITSTATUS(child_status);
			data->status = signal;
		}
		i++;
	}
	free_fds_bonus(data);
	return(signal);
}

void	free_fds_bonus(t_data	*data)
{
	closing_files(&data->fds);
	if (data->fds->fd_files)
		free(data->fds->fd_files);
	if (data->fds->c_pids)
		free(data->fds->c_pids);
	data->fds->fd_files = NULL;
	data->fds->c_pids = NULL;
}