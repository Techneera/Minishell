#include "execution.h"


int	main(int argc, char *argv[], char **envp)
{
	t_ast	*cmd = fail_cmd();
	t_fds	*fds;
	int		child_status;
	int	g_signal;
	int	i;


	fds = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!cmd)
		return (-1);
	create_fds(&fds, cmd);
	execute_tree(cmd, &fds, -1, envp);
	ft_closing_all(&fds);
	free_all((void **) fds->pipe_fds, number_of_cmds(cmd) - 1);
	free_all((void **) fds->heredoc_fds, fds->n_docs);
	free(fds->fd_files);
	free(fds);
	free_tree(cmd);
	i = 0;
	while(waitpid(fds->c_pids[i], &child_status, 0) > 0)
	{
		if (WIFEXITED(child_status))
			g_signal = WEXITSTATUS(child_status);
		i++;
	}
	return(g_signal);
}
