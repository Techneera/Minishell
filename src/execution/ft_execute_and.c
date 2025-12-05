#include "execution.h"

static int	redir_to_or(t_data	*data, char **envp);
static int	redir_to_and(t_data	*data, char **envp);
static void	redir_bonus(t_data	*data, char **envp);

int	execute_and(t_data	*data, char **envp)
{
	t_ast	*node;

	free_fds_bonus(data);
	node = data->tree;
	if (node == NULL)
		return (0);
	ft_create_fds_bonus(data);
	if (node->type == NODE_CMD)
		ft_execute_cmd(data);
	if (node->type == NODE_PIPE || node->type == NODE_SUBSHELL)
		ft_exec_tree(data, envp);
	closing_files(&data->fds);
	if (data->fds && data->fds->c_pids)
		return (wait_bonus(data, data->fds));
	else
		redir_bonus(data, envp);
	return (0);
}

static void	redir_bonus(t_data	*data, char **envp)
{
	if (data->fds->fd_files)
		free(data->fds->fd_files);
	if (data->fds->c_pids)
		free(data->fds->c_pids);
	if (data->tree->type == NODE_AND)
	{
		if (redir_to_and(data, envp) == -1)
			return ;
	}
	if (data->tree->type == NODE_OR)
	{
		if (redir_to_or(data, envp) == -1)
			return ;
	}
}

static int	redir_to_and(t_data	*data, char **envp)
{
	t_ast	*holder;

	holder = data->tree;
	if (holder->left)
	{
		data->tree = holder->left;
		if (execute_and(data, envp) != 0)
			return (free_fds_bonus(data), -1);
	}
	if (holder->right)
	{
		data->tree = holder->right;
		if (execute_and(data, envp) != 0)
			return (free_fds_bonus(data), -1);
	}
	data->tree = holder;
	return (0);
}

static int	redir_to_or(t_data	*data, char **envp)
{
	t_ast	*holder;

	holder = data->tree;
	if (holder->left)
	{
		data->tree = holder->left;
		if (execute_or(data, envp) == 0)
			return (free_fds_bonus(data), -1);
	}
	if (holder->right)
	{
		data->tree = holder->right;
		if (execute_or(data, envp) == 0)
			return (free_fds_bonus(data), -1);
	}
	data->tree = holder;
	return (0);
}
