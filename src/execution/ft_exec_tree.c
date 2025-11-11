#include "execution.h"

static int	execute_pipe(t_data	*data, char **envp);
static int	execute_cmd(t_data	*data, char **envp);
static int	execute_and(t_data	*data, char **envp);
static int	execute_or(t_data	*data, char **envp);

int	ft_exec_tree(t_data	*data, char **envp)
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
		execute_cmd(data, envp);
		if (fds->pos.pipe_id < fds->get.n_pipes)
		{
			if (fds->pos.pipe_id > 1)
				secure_close(&fds->pipe_fds[fds->pos.pipe_id - 1][0]);
			if (fds->pos.pipe_id == fds->get.n_pipes)
				secure_close(&fds->pipe_fds[fds->pos.pipe_id][0]);
			secure_close(&fds->pipe_fds[fds->pos.pipe_id][1]);
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
		fds->pos.pipe_id++;
	}
	else if (node->type == NODE_PIPE)
		execute_pipe(data, envp);
	else if (node->type == NODE_SUBSHELL)
	{
		t_data holder;

		holder = *data;
		holder.tree = node->body;
		apply_redirs_subshell(data);
		ft_exec_tree(&holder, envp);
		dup2(STDIN_FILENO, STDIN_FILENO);
		dup2(STDOUT_FILENO, STDOUT_FILENO);
	}
	else if (node->type == NODE_AND)
		execute_and(data, envp);
	else if (node->type == NODE_OR)
	 	execute_or(data, envp);
	return (0);
}

static int	execute_pipe(t_data	*data, char **envp)
{
	t_ast	*holder;

	holder = data->tree;
	if (holder->left)
	{
		data->tree = holder->left;
		ft_exec_tree(data, envp);
	}
	if (holder->right)
	{
		data->tree = holder->right;
		ft_exec_tree(data, envp);
	}
	return (0);
}

static int	execute_cmd(t_data	*data, char **envp)
{
	pid_t	pid;
	t_fds	*fds;
	t_ast	*node;
	int		j;

	fds = data->fds;
	node = data->tree;
	j = 0;
	if (!data->tree)
		return (0);
	if (!init_pid(&pid, &fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
		ft_child_process(data, envp);
	while (j < node->cmd->redir_count)
	{
		if (node->cmd->redirs[j].label != REDIR_HEREDOCK)
			fds->pos.file_id++;
		else
			fds->pos.doc_id++;
		j++;
	}
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
			ft_child_process(&new_data, envp);
		}
	}
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
			ft_child_process(&new_data, envp);
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
