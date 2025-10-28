#include "execution.h"

int	init_pid(pid_t *pid, t_fds **fds)
{
	int	i;

	i = 0;
	*pid = fork();
	if (*pid != 0)
	{
		while ((*fds)->c_pids[i])
			i++;
		(*fds)->c_pids[i] = *pid;
	}
	if (*pid == -1)
	{
		perror("fork");
		return (0);
	}
	return (1);
}
