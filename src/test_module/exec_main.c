#include "execution.h"

int child_status;

int	main(int argc, char *argv[], char **envp)
{
	t_ast	*cmd = bonus_cmd();
	t_fds	*fds;
	int	g_signal;
	int	i;

	fds = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!cmd)
		return (-1);
	ft_create_fds(&fds, cmd);
	ft_exec_tree(cmd, &fds, -1, envp);
	ft_closing_all(&fds);
	i = 0;
	while(fds->c_pids && waitpid(fds->c_pids[i], &child_status, 0) > 0)
	{
		if (WIFEXITED(child_status))
			g_signal = WEXITSTATUS(child_status);
		i++;
	}
	free_all((void **) fds->pipe_fds, fds->get.n_pipes);
	free_all((void **) fds->heredoc_fds, fds->get.n_docs);
	free(fds->fd_files);
	free(fds);
	free_tree(cmd);
	return(g_signal);
}
