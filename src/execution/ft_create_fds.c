#include "execution.h"

static void	init_pipe(t_fds **fds, int ***pipe_fds);
static void	create_pipe(t_fds **fds, int ***pipe_fds);

void	ft_create_fds(t_fds **fds, t_ast *ast_root)
{
	*fds = malloc(sizeof(t_fds));
	(*fds)->get.n_files = 0;
	(*fds)->pipe_fds = 0;
	if (!*fds)
		exit(1);
	(*fds)->pos.file_id = 0;
	get_sizes(ast_root, fds);
	init_pipe(fds, &((*fds)->pipe_fds));
	if ((*fds)->get.n_files > 0)
	{
		(*fds)->fd_files = malloc((*fds)->get.n_files * sizeof(int));
		if (!(*fds)->fd_files)
			exit(1);
	}
	if ((*fds)->get.n_cmds > 0)
	{
		(*fds)->c_pids = ft_calloc((*fds)->get.n_cmds, sizeof(int));
		if (!(*fds)->c_pids)
			exit(1);
	}
	init_heredoc(fds, ast_root);
	fill_fd_file(fds, ast_root, 0);
}

static void	init_pipe(t_fds **fds, int ***pipe_fds)
{
	if ((*fds)->get.n_pipes > 0)
	{
		*pipe_fds = malloc((*fds)->get.n_pipes * sizeof(int *));
		if (!*pipe_fds)
			exit(1);
		create_pipe(fds, pipe_fds);
	}
}

static void	create_pipe(t_fds **fds, int ***pipe_fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->get.n_pipes)
	{
		(*pipe_fds)[i] = malloc(2 * sizeof(int));
		if (!(*pipe_fds)[i])
		{
			free_all((void **) *pipe_fds, (*fds)->get.n_pipes);
			exit(1);
		}
		if (pipe((*pipe_fds)[i]) == -1)
		{
			free_all((void **) *pipe_fds, (*fds)->get.n_pipes);
			exit(1);
		}
		i++;
	}
}
