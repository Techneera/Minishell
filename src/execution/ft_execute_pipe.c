#include "execution.h"

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds);

void ft_execute_pipe(t_data *data, char **envp)
{
	int		pfd[2];
	pid_t	pipe_pid[2];
	t_ast	*holder;
	int		status;
	int		i;
	int		n_cmds;

	i = 0;
	status = 0;
	holder = data->tree;
	if (pipe(pfd) == -1)
		return ;
	if (!init_pid(&pipe_pid[0], &data->fds))
	{
		close(pfd[0]);
		close(pfd[1]);
		secure_exit(data, FAIL_STATUS);
	}
	if (pipe_pid[0] == 0)
	{
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[0]);
		close(pfd[1]);
		data->tree = holder->left;
		n_cmds = ft_n_cmds_sshell(data->tree, &data->fds);
		if (data->fds->c_pids)
			free(data->fds->c_pids);
		data->fds->c_pids = ft_calloc(n_cmds, sizeof(int));
		if (!data->fds->c_pids)
			secure_exit(data, 0);
		data->fds->pos.fork_id = 0;
		ft_exec_tree(data, envp);
		while(data->fds && i < n_cmds && waitpid(data->fds->c_pids[i], &status, 0) > 0)
		{	
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
			i++;
		}
		secure_exit(data, data->status);
	}
	update_from_position(data->tree->left, data);
	if (!init_pid(&pipe_pid[1], &data->fds))
	{
		close(pfd[0]);
		close(pfd[1]);
		secure_exit(data, FAIL_STATUS);
	}
	if (pipe_pid[1] == 0)
	{
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		close(pfd[1]);
		data->tree = holder->right;
		n_cmds = ft_n_cmds_sshell(data->tree, &data->fds);
		if (data->fds->c_pids)
			free(data->fds->c_pids);
		data->fds->c_pids = ft_calloc(n_cmds, sizeof(int));
		if (!data->fds->c_pids)
			secure_exit(data, 0);
		data->fds->pos.fork_id = 0;
		ft_exec_tree(data, envp);
		while(data->fds && i < n_cmds && waitpid(data->fds->c_pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
			i++;
		}
		secure_exit(data, data->status);
	}
	update_from_position(data->tree->right, data);
	close(pfd[0]);
	close(pfd[1]);
}

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds)
{
	int	i;

	i = 0;
	if (!ast_root)
		return (i);
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
		return (i);
	i += ft_n_cmds_sshell(ast_root->left, fds);
	i += ft_n_cmds_sshell(ast_root->right, fds);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
		i++;
	return (i);
}
