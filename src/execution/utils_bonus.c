#include "execution.h"

int	wait_bonus(t_data *data, t_fds *fds)
{
	int	signal;
	int	i;
	int	status;

	signal = 0;
	i = 0;
	while(i < fds->get.n_cmds && fds->c_pids && waitpid(fds->c_pids[i], &status, 0) > 0)
	{
		if (WIFEXITED(status))
			ft_exit_status(WEXITSTATUS(status), 1, 0);
		i++;
	}
	free_fds_bonus(data);
	return(ft_exit_status(0, 0, 0));
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