#include "execution.h"

static void	update_positions(t_data *data);
static void	update_pos_pipe(t_data *data);
static void	update_pos_files(t_data *data);

void	ft_execute_cmd(t_data *data, char **envp)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	if (!init_pid(&pid, &data->fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
	{
		ft_child_process(data, envp);
		secure_exit(data, 42);
	}
	update_positions(data);
}

static void	update_positions(t_data *data)
{
	update_pos_pipe(data);
	update_pos_files(data);
}

static void	update_pos_pipe(t_data *data)
{
	t_fds	*fds;

	fds = data->fds;
	if (fds->pos.pipe_id < fds->get.n_pipes)
	{
		if (fds->pos.pipe_id > 1)
			secure_close(&fds->pipe_fds[fds->pos.pipe_id - 1][0]);
		if (fds->pos.pipe_id == fds->get.n_pipes)
			secure_close(&fds->pipe_fds[fds->pos.pipe_id][0]);
		secure_close(&fds->pipe_fds[fds->pos.pipe_id][1]);
	}
	fds->pos.pipe_id++;
}

static void	update_pos_files(t_data *data)
{
	t_fds	*fds;
	t_ast	*node;
	int		j;

	j = 0;
	fds = data->fds;
	node = data->tree;
	while (j < node->cmd->redir_count)
	{
		if (node->cmd->redirs[j].label != REDIR_HEREDOCK)
		{
			secure_close(&fds->fd_files[fds->pos.file_id]);
			fds->pos.file_id++;
		}
		else
		{
			secure_close(&fds->heredoc_fds[fds->pos.doc_id][0]);
			secure_close(&fds->heredoc_fds[fds->pos.doc_id][1]);
			fds->pos.doc_id++;
		}
		j++;
	}
}