#include "execution.h"

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds);

void	ft_child_sshell(t_data *data, char **envp)
{
	int		n_cmds;
	int		i;

	i = 0;
	n_cmds = ft_n_cmds_sshell(data->tree->body, &data->fds);
	apply_redirs_subshell(data);
	if (data->fds->c_pids)
		free(data->fds->c_pids);
	data->fds->c_pids = ft_calloc(n_cmds, sizeof(int));
	if (!data->fds->c_pids)
		secure_exit(data, 0);
	data->tree = data->tree->body;
	ft_exec_tree(data, envp);
	while(data->fds && i < n_cmds && waitpid(data->fds->c_pids[i], &data->status, 0) > 0)
	{	
		if (WIFEXITED(child_status))
			data->status = WEXITSTATUS(child_status);
		i++;
	}
	secure_exit(data, data->status);
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
