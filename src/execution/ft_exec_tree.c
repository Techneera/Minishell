#include "execution.h"

static int	execute_pipe(t_ast *node, t_fds **fds, int i, char **envp);
static int	execute_cmd(t_ast *node, t_fds **fds, int i, char **envp);
static int	execute_subshell(t_ast *node, t_fds **fds, int i, char **envp);
// static int	execute_and(t_ast *node, t_fds **fds, int i, char **envp);
// static int	execute_or(t_ast *node, t_fds **fds, int i, char **envp);

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
		i = execute_subshell(node, fds, i, envp);
	// else if (node->type == NODE_AND)
	// 	i = execute_and(node, fds, i, envp);
	// else if (node->type == NODE_OR)
	// 	i = execute_or(node, fds, i, envp);
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
	if (i == -1)
		i = 0;
	if (!init_pid(&pid, fds))
		exit(1);
	if (pid == 0)
		ft_child_process(node, fds, i, envp);
	return (i);
}

// static	int	execute_bonus(t_ast *node, t_fds **fds, int i, char **envp)
// {
// 	int	**new_pipes;
// 	int	start_point;
// 	int	end_point;

// 	start_point = i;
// 	if (node->body->type == NODE_AND || node->body->type == NODE_OR)
// 	{
// 		search_and_create_pipes(&new_pipes);
// 	}
// 	else
// 	{
// 		end_point = ft_exec_tree(node->body, fds, i, envp);
// 	}
// }

void	heredoc_subshell(t_cmd *cmd, t_fds **fds, int r)
{
	if (cmd->redirs[r].label == REDIR_HEREDOCK)
	{
		if ((*fds)->heredoc_fds[(*fds)->pos.doc_id][0] != -1
		&& dup2((*fds)->heredoc_fds[(*fds)->pos.doc_id][0], STDIN_FILENO) == -1)
			perror("dup on REDIR_DOC");
		(*fds)->pos.doc_id++;
	}
}
void	apply_redirs_subshell(t_cmd *cmd, t_fds **fds)
{
	int	r;

	r = 0;
	while (r < cmd->redir_count)
	{
		heredoc_subshell(cmd, fds, r);
		if (cmd->redirs[r].label == REDIR_IN)
		{
			if (dup2((*fds)->fd_files[(*fds)->pos.file_id], STDIN_FILENO) == -1)
				exit(1);
			(*fds)->pos.file_id++;
		}
		else if (cmd->redirs[r].label == REDIR_OUT
			|| cmd->redirs[r].label == REDIR_APPEND)
		{
			if ((*fds)->fd_files[(*fds)->pos.file_id] != -1
				&& dup2((*fds)->fd_files[(*fds)->pos.file_id], STDOUT_FILENO) == -1)
				perror("dup on REDIR_OUT");
			(*fds)->pos.file_id++;
		}
		r++;
	}
}



static int	execute_subshell(t_ast *node, t_fds **fds, int i, char **envp)
{
	if (node->body->type == NODE_PIPE)
	{
		apply_redirs_subshell(node->cmd, fds);
		ft_exec_tree(node->body, fds, i, envp);
	}
	return (0);
}

// static void	execute_and(t_ast *node, t_fds **fds, int i, char **envp)
// {
// 	pid_t pid;

// 	if (node == NULL)
// 		return ;

// 	execute_and(node->left, fds, i, envp);
// 	execute_and(node->right, fds, i,envp);
// 	if (node->type == NODE_CMD)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork ERROR");
// 			exit(1);
// 		}
// 		if (pid == 0)
// 			ft_execute_cmd();
// 		waitpid(pid, &child_status, 0);
// 		if (WIFEXITED(child_status) && WEXITSTATUS(child_status) != 1)
// 			return ;
// 	}
// }

// static int	execute_or(t_ast *node, t_fds **fds, int i, char **envp)
// {
// 	pid_t pid;

// 	if (node == NULL)
// 		return ;

// 	execute_and(node->left, fds, i, envp);
// 	execute_and(node->right, fds, i,envp);
// 	if (node->type == NODE_CMD)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork ERROR");
// 			exit(1);
// 		}
// 		if (pid == 0)
// 			ft_execute_cmd();
// 		waitpid(pid, &child_status, 0);
// 		if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 1)
// 			return ;
// 	}
