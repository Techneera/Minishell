#include "execution.h"

static void	init_pipe(t_data *data, t_fds **fds, int ***pipe_fds);
static void	create_pipe(t_data *data, t_fds **fds, int ***pipe_fds);

void	ft_create_fds(t_data *data)
{
	t_fds	*fds;
	t_ast	*ast_root;
	int		i;

	i = -1;
	ast_root = data->tree;
	fds = ft_calloc(1, sizeof(t_fds));
	if (!fds)
		secure_exit(data, FAIL_STATUS);
	fds->pos.file_id = 0;
	get_sizes(ast_root, &fds);
	init_pipe(data, &fds, &(fds->pipe_fds));
	if (fds->get.n_files > 0)
	{
		fds->fd_files = ft_calloc(fds->get.n_files, sizeof(int));
		if (!fds->fd_files)
			secure_exit(data, FAIL_STATUS);
		while (++i < fds->get.n_files)		
			fds->fd_files[i] = -1;
	}
	if (fds->get.n_cmds > 0)
	{
		fds->c_pids = ft_calloc(fds->get.n_cmds, sizeof(int));
		if (!fds->c_pids)
			secure_exit(data, FAIL_STATUS);
	}
	data->fds = fds;
	init_heredoc(data);
	fill_fd_file(&data->fds, ast_root, 0);
}

static void	init_pipe(t_data *data, t_fds **fds, int ***pipe_fds)
{
	if ((*fds)->get.n_pipes > 0)
	{
		*pipe_fds = malloc((*fds)->get.n_pipes * sizeof(int *));
		if (!*pipe_fds)
			secure_exit(data, FAIL_STATUS);
		create_pipe(data, fds, pipe_fds);
	}
}

static void	create_pipe(t_data *data, t_fds **fds, int ***pipe_fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->get.n_pipes)
	{
		(*pipe_fds)[i] = malloc(2 * sizeof(int));
		if (!(*pipe_fds)[i])
		{
			free_all((void **) *pipe_fds, (*fds)->get.n_pipes);
			secure_exit(data, FAIL_STATUS);
		}
		if (pipe((*pipe_fds)[i]) == -1)
		{
			free_all((void **) *pipe_fds, (*fds)->get.n_pipes);
			secure_exit(data, FAIL_STATUS);
		}
		i++;
	}
}
