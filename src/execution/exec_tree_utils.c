#include "execution.h"

int	init_pid(pid_t *pid, t_fds **fds)
{
	*pid = fork();
	if (*pid != 0)
	{
		(*fds)->c_pids[(*fds)->pos.fork_id] = *pid;
		(*fds)->pos.fork_id++;
	}
	if (*pid == -1)
	{
		perror("fork");
		return (0);
	}
	return (1);
}
