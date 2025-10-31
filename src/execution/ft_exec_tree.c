#include "execution.h"

static int	execute_pipe(t_ast *node, t_fds **fds, int i, char **envp);
static int	execute_cmd(t_ast *node, t_fds **fds, int i, char **envp);
static int	execute_and(t_ast *node, t_fds **fds, int i, char **envp);
static int	execute_or(t_ast *node, t_fds **fds, int i, char **envp);

int	ft_exec_tree(t_ast *node, t_fds **fds, int i, char **envp)
{
	(void)envp;
	if (node == NULL)
		return (0);
	if (node->type == NODE_CMD)
	{
		i = execute_cmd(node, fds, i, envp);
		(*fds)->pos.file_id += node->cmd->redir_count;
		i++;
	}
	else if (node->type == NODE_PIPE)
		i = execute_pipe(node, fds, i, envp);
	else if (node->type == NODE_SUBSHELL)
	{
		apply_redirs_subshell(node->cmd, fds);
		i = ft_exec_tree(node->body, fds, i, envp);
		dup2(STDIN_FILENO, STDIN_FILENO);
		dup2(STDOUT_FILENO, STDOUT_FILENO);
	}
	else if (node->type == NODE_AND)
		i = execute_and(node, fds, i, envp);
	else if (node->type == NODE_OR)
	 	i = execute_or(node, fds, i, envp);
	return (i);
}

static int	execute_pipe(t_ast *node, t_fds **fds, int i, char **envp)
{
	(void)envp;
	if (node->left)
		i = ft_exec_tree(node->left, fds, i, envp);
	if (node->right)
		i = ft_exec_tree(node->right, fds, i, envp);
	return (i);
}

static int	execute_cmd(t_ast *node, t_fds **fds, int i, char **envp)
{
	pid_t	pid;

	if (!node)
		return (i);
	if (!init_pid(&pid, fds))
		exit(1);
	if (pid == 0)
		ft_child_process(node, fds, i, envp);
	return (i);
}

static int	wait_bonus(t_fds *new_fds, int i)
{
	int		signal;

	signal = 0;
	while(waitpid(new_fds->c_pids[i], &child_status, 0) > 0)
	{
		if (WIFEXITED(child_status))
			signal = WEXITSTATUS(child_status);
		i++;
	}
	return(signal);
}
static int	execute_and(t_ast *node, t_fds **fds, int i, char **envp)
{
	t_fds	*new_fds;
	pid_t pid;

	new_fds = NULL;
	if (node == NULL)
		return (0);
	ft_create_fds(&new_fds, node);
	if (node->type == NODE_CMD)
	{
		if (!init_pid(&pid, &new_fds))
			exit(1);
		if (pid == 0)
			ft_child_process(node, &new_fds, i, envp);
	}
	if (node->type == NODE_PIPE)
		ft_exec_tree(node, &new_fds, -1, envp);
	ft_closing_all(&new_fds);
	if (new_fds && new_fds->c_pids)
		return (wait_bonus(new_fds, i));
	if (execute_and(node->left, fds, i, envp) != 0)
		return (free_fds(new_fds), -1);
	if (execute_and(node->right, fds, i,envp) != 0)	
		return (free_fds(new_fds), -1);
	return (0);
}

static int	execute_or(t_ast *node, t_fds **fds, int i, char **envp)
{
	t_fds	*new_fds;
	pid_t pid;

	new_fds = NULL;
	if (node == NULL)
		return (0);
	ft_create_fds(&new_fds, node);
	if (node->type == NODE_CMD)
	{
		if (!init_pid(&pid, &new_fds))
			exit(1);
		if (pid == 0)
			ft_child_process(node, &new_fds, i, envp);
	}
	if (node->type == NODE_PIPE)
		ft_exec_tree(node, &new_fds, -1, envp);
	ft_closing_all(&new_fds);
	wait_bonus(new_fds, i);
	if (execute_or(node->left, fds, i, envp) == 0)
		return (free_fds(new_fds), -1);
	if (execute_or(node->right, fds, i,envp) == 0)
		return (free_fds(new_fds), -1);
	return (0);
}
