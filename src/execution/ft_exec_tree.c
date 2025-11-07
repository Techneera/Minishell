#include "execution.h"

static int	execute_pipe(t_data	*data, int i, char **envp);
static int	execute_cmd(t_data	*data, int i, char **envp);
static int	execute_and(t_data	*data, int i, char **envp);
static int	execute_or(t_data	*data, int i, char **envp);

int	ft_exec_tree(t_data	*data, int i, char **envp)
{
	int	old_pos_file;
	int	old_pos_doc;
	t_fds	*fds;
	t_ast	*node;

	fds = data->fds;
	node = data->tree;
	old_pos_file = fds->pos.file_id;
	old_pos_doc = fds->pos.doc_id;
	if (node == NULL || !fds)
		return (0);
	if (node->type == NODE_CMD)
	{
		i = execute_cmd(data, i, envp);
		if (i < fds->get.n_pipes)
		{
			if (i > 1)
				secure_close(&fds->pipe_fds[i - 1][0]);
			if (i == fds->get.n_pipes)
				secure_close(&fds->pipe_fds[i][0]);
			secure_close(&fds->pipe_fds[i][1]);
		}
		while (old_pos_file < fds->pos.file_id) 
		{
			secure_close(&fds->fd_files[old_pos_file]);
			old_pos_file++;
		}
		while (old_pos_doc < fds->pos.doc_id) 
		{
			secure_close(&fds->heredoc_fds[old_pos_doc][0]);
			secure_close(&fds->heredoc_fds[old_pos_doc][1]);
			old_pos_doc++;
		}
		i++;
	}
	else if (node->type == NODE_PIPE)
		i = execute_pipe(data, i, envp);
	else if (node->type == NODE_SUBSHELL)
	{
		apply_redirs_subshell(data);
		i = ft_exec_tree(data, i, envp);
		dup2(STDIN_FILENO, STDIN_FILENO);
		dup2(STDOUT_FILENO, STDOUT_FILENO);
	}
	else if (node->type == NODE_AND)
		i = execute_and(data, i, envp);
	else if (node->type == NODE_OR)
	 	i = execute_or(data, i, envp);
	return (i);
}

static int	execute_pipe(t_data	*data, int i, char **envp)
{
	t_ast	*holder;

	holder = data->tree;
	if (holder->left)
	{
		data->tree = holder->left;
		i = ft_exec_tree(data, i, envp);
	}
	if (holder->right)
	{
		data->tree = holder->right;
		i = ft_exec_tree(data, i, envp);
	}
	return (i);
}

static int	execute_cmd(t_data	*data, int i, char **envp)
{
	pid_t	pid;
	t_fds	*fds;
	t_ast	*node;
	int		j;

	fds = data->fds;
	node = data->tree;
	j = 0;
	if (!data->tree)
		return (i);
	if (!init_pid(&pid, &fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
		ft_child_process(data, i, envp);
	while (j < node->cmd->redir_count)
	{
		if (node->cmd->redirs[j].label != REDIR_HEREDOCK)
			fds->pos.file_id++;
		else
			fds->pos.doc_id++;
		j++;
	}
	return (i);
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

static int	execute_and(t_data	*data, int i, char **envp)
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
			ft_child_process(&new_data, i, envp);
		}
	}
	if (node->type == NODE_PIPE)
		ft_exec_tree(&new_data, -1, envp);
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
			i = execute_and(&new_data, i, envp);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			i = execute_and(&new_data, i, envp);
		}
	}
	if (new_data.tree->type == NODE_OR)
	{
		t_ast	*holder;

		holder = new_data.tree;
		if (holder->left)
		{
			new_data.tree = holder->left;
			if (execute_and(&new_data, i, envp) == 0)
				return (free_fds(&new_data.fds), -1);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			if (execute_and(&new_data, i, envp) == 0)
				return (free_fds(&new_data.fds), -1);
		}		
	}
	return (0);
}

static int	execute_or(t_data	*data, int i, char **envp)
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
			ft_child_process(&new_data, i, envp);
		}
	}
	if (node->type == NODE_PIPE)
		ft_exec_tree(&new_data, -1, envp);
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
			i = execute_and(&new_data, i, envp);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			i = execute_and(&new_data, i, envp);
		}
	}
	if (new_data.tree->type == NODE_OR)
	{
		t_ast	*holder;

		holder = new_data.tree;
		if (holder->left)
		{
			new_data.tree = holder->left;
			if (execute_and(&new_data, i, envp) == 0)
				return (free_fds(&new_data.fds), -1);
		}
		if (holder->right)
		{
			new_data.tree = holder->right;
			if (execute_and(&new_data, i, envp) == 0)
				return (free_fds(&new_data.fds), -1);
		}		
	}
	return (0);
}
