#include "execution.h"

static int	execute_and(t_data	*data, char **envp);
static int	execute_or(t_data	*data, char **envp);

int	ft_exec_tree(t_data	*data, char **envp)
{
	t_fds	*fds;
	t_ast	*node;

	fds = data->fds;
	node = data->tree;
	if (node == NULL || !fds)
		return (0);
	if (node->type == NODE_CMD)
		ft_execute_cmd(data, envp);
	else if (node->type == NODE_PIPE)
		ft_execute_pipe(data, envp);
	else if (node->type == NODE_SUBSHELL)
		ft_execute_sshell(data, envp);
	else if (node->type == NODE_AND)
		execute_and(data, envp);
	else if (node->type == NODE_OR)
	 	execute_or(data, envp);
	return (0);
}

static int	wait_bonus(t_fds *new_fds)
{
	int	signal;
	int	i;

	signal = 0;
	i = 0;
	while(i < new_fds->get.n_cmds && waitpid(new_fds->c_pids[i], &child_status, 0) > 0)
	{
		if (WIFEXITED(child_status))
			signal = WEXITSTATUS(child_status);
		i++;
	}
	return(signal);
}

static int	execute_and(t_data	*data, char **envp)
{
	t_data	new_data;
	t_ast	*node;

	ft_closing_all(&data->fds);
	free_fds(&data->fds);
	new_data = *data;
	node = new_data.tree;
	if (node == NULL)
		return (0);
	ft_create_fds(&new_data);	
	if (node->type == NODE_CMD)
		ft_execute_cmd(&new_data, envp);
	if (node->type == NODE_PIPE || node->type == NODE_SUBSHELL)
		ft_exec_tree(&new_data, envp);
	ft_closing_all(&new_data.fds);
	if (new_data.fds && new_data.fds->c_pids)
	{
		int value = wait_bonus(new_data.fds);
		free_fds(&new_data.fds);
		return (value);	
	}
	free_fds(&new_data.fds);
	if (new_data.tree->type == NODE_AND)
	{
		t_ast	*holder;

		holder = new_data.tree;
		if (holder->left)
		{
			new_data.tree = holder->left;
			if (execute_and(&new_data, envp) != 0)
				return (free_fds(&new_data.fds), -1);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			if (execute_and(&new_data, envp) != 0)
				return (free_fds(&new_data.fds), -1);
		}		
	}
	if (new_data.tree->type == NODE_OR)
	{
		t_ast	*holder;

		holder = new_data.tree;
		if (holder->left)
		{
			new_data.tree = holder->left;
			if (execute_or(&new_data, envp) == 0)
				return (free_fds(&new_data.fds), 0);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			if (execute_or(&new_data, envp) == 0)
				return (free_fds(&new_data.fds), 0);
		}		
	}
	return (0);
}

static int	execute_or(t_data	*data, char **envp)
{
	t_data	new_data;
	t_ast	*node;
	pid_t 	pid;

	new_data = *data;
	node = new_data.tree;
	if (node == NULL)
		return (0);
	ft_create_fds(&new_data);
	if (node->type == NODE_CMD)
	{
		if (!init_pid(&pid, &new_data.fds))
		{
			free_fds(&new_data.fds);
			secure_exit(data, 1);
		}
		if (pid == 0)
		{
			free_fds(&data->fds);
			ft_child_cmd(&new_data, envp);
		}
	}
	if (node->type == NODE_PIPE || node->type == NODE_SUBSHELL)
		ft_exec_tree(&new_data, envp);
	ft_closing_all(&new_data.fds);
	wait_bonus(new_data.fds);
	free_fds(&new_data.fds);
	if (new_data.tree->type == NODE_AND)
	{
		t_ast	*holder;

		holder = new_data.tree;
		if (holder->left)
		{
			new_data.tree = holder->left;
			if (execute_and(&new_data, envp) != 0)
				return (free_fds(&new_data.fds), -1);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			if (execute_and(&new_data, envp) != 0)
				return (free_fds(&new_data.fds), -1);
		}		
	}
	if (new_data.tree->type == NODE_OR)
	{
		t_ast	*holder;

		holder = new_data.tree;
		if (holder->left)
		{
			new_data.tree = holder->left;
			if (execute_or(&new_data, envp) == 0)
				return (free_fds(&new_data.fds), 0);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			if (execute_or(&new_data, envp) == 0)
				return (free_fds(&new_data.fds), 0);
		}		
	}
	return (0);
}
