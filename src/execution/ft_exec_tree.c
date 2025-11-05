#include "execution.h"

static int	execute_pipe(t_data	*data, int i, char **envp);
static int	execute_cmd(t_data	*data, int i, char **envp);
// static int	execute_and(t_ast **node, t_fds **fds, int i, char **envp);
//static int	execute_or(t_ast **node, t_fds **fds, int i, char **envp);

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
			secure_close(&fds->fd_files[old_pos_doc]);
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
	// else if ((*node)->type == NODE_AND)
	// 	i = execute_and(node, fds, i, envp);
	// else if ((*node)->type == NODE_OR)
	//  	i = execute_or(node, fds, i, envp);
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

// static int	wait_bonus(t_fds *new_fds)
// {
// 	int	signal;
// 	int	i;

// 	signal = 0;
// 	i = 0;
// 	while(i < new_fds->get.n_cmds && waitpid(new_fds->c_pids[i], &child_status, 0) > 0)
// 	{
// 		if (WIFEXITED(child_status))
// 			signal = WEXITSTATUS(child_status);
// 		i++;
// 	}
// 	return(signal);
//}

// static int	execute_and(t_ast **node, t_fds **fds, int i, char **envp)
// {
// 	t_fds	*new_fds;
// 	pid_t pid;

// 	new_fds = NULL;
// 	if (node == NULL)
// 		return (0);
// 	ft_create_fds(&new_fds, *node);
// 	if ((*node)->type == NODE_CMD)
// 	{
// 		if (!init_pid(&pid, &new_fds))
// 		{
// 			free_fds(&new_fds);
// 			exit(1);
// 		}
// 		if (pid == 0)
// 		{
// 			free_fds(fds);
// 			ft_child_process(node, &new_fds, i, envp);
// 		}
// 	}
// 	if ((*node)->type == NODE_PIPE)
// 		ft_exec_tree(node, &new_fds, -1, envp);
// 	ft_closing_all(&new_fds);
// 	if (new_fds && new_fds->c_pids)
// 	{
// 		int value = wait_bonus(new_fds);
// 		free_fds(&new_fds);
// 		return (value);	
// 	}
// 	free_fds(&new_fds);
// 	if (execute_and(&(*node)->left, fds, i, envp) != 0)
// 		return (-1);
// 	if (execute_and(&(*node)->right, fds, i,envp) != 0)	
// 		return (-1);
// 	return (0);
// }

// static int	execute_or(t_ast **node, t_fds **fds, int i, char **envp)
// {
// 	t_fds	*new_fds;
// 	pid_t pid;

// 	new_fds = NULL;
// 	if (node == NULL)
// 		return (0);
// 	ft_create_fds(&new_fds, node);
// 	if ((*node)->type == NODE_CMD)
// 	{
// 		if (!init_pid(&pid, &new_fds))
// 			exit(1);
// 		if (pid == 0)
// 			ft_child_process(node, &new_fds, i, envp);
// 	}
// 	if ((*node)->type == NODE_PIPE)
// 		ft_exec_tree(node, &new_fds, -1, envp);
// 	ft_closing_all(&new_fds);
// 	wait_bonus(new_fds);
// 	if (execute_or((*node)->left, fds, i, envp) == 0)
// 		return (free_fds(new_fds), -1);
// 	if (execute_or((*node)->right, fds, i,envp) == 0)
// 		return (free_fds(new_fds), -1);
// 	return (0);
// }
