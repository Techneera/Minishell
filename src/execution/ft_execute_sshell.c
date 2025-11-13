#include "execution.h"

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds);

void	ft_execute_sshell(t_data *data, char **envp)
{
	pid_t	pid;
	int		i;
	int		signal;
	int		n_cmds;

	i = 0;
	signal = 0;
	n_cmds = ft_n_cmds_sshell(data->tree->body, &data->fds);
	if (!data->tree)
		return ;
	if (!init_pid(&pid, &data->fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
	{
		apply_std_dup(data);
		apply_redirs_subshell(data);
		if (data->fds->c_pids)
			free(data->fds->c_pids);
		data->fds->c_pids = ft_calloc(n_cmds, sizeof(int));
		if (!data->fds->c_pids)
			secure_exit(data, 0);
		data->tree = data->tree->body;
		ft_exec_tree(data, envp);
		while(data->fds && i < n_cmds && waitpid(data->fds->c_pids[i], &child_status, 0) > 0)
		{
			if (WIFEXITED(child_status))
				signal = WEXITSTATUS(child_status);
			i++;
		}
		secure_exit(data, signal);
	}
	update_positions(data);
}

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds)
{
	int	i;

	i = 0;
	if (!ast_root)
		return (0);
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
		return (0);
	i += ft_n_cmds_sshell(ast_root->left, fds);
	i += ft_n_cmds_sshell(ast_root->right, fds);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
		i++;
	return (i);
}