#include "execution.h"

int	main(int argc, char *argv[], char **envp)
{
	t_ast	*cmd = ft_cmd4();
	t_fds	*fds;

	fds = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!cmd)
		return (-1);
	create_fds(&fds, cmd);
	execute_tree(cmd, &fds, -1, envp);
	ft_closing_all(&fds);
	while (waitpid(-1, NULL, 0) > 0)
		;
	free_all((void **) fds->pipe_fds, number_of_cmds(cmd) - 1);
	free_all((void **) fds->heredoc_fds, fds->n_docs);
	free(fds->fd_files);
	free(fds);
	free_tree(cmd);
}
