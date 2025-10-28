#include "execution.h"

static int	execute_pipe(t_ast *node, t_fds **fds, int i, char **envp);
static int	execute_cmd(t_ast *node, t_fds **fds, int i, char **envp);
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

static	int	execute_bonus(t_ast *node, t_fds **fds, int i, char **envp)
{
	int	**new_pipes;
	int	start_point;
	int	end_point;

	start_point = i;
	if (node->body->type == NODE_AND || node->body->type == NODE_OR)
	{
		search_and_create_pipes(&new_pipes);
	}
	else
	{
		end_point = ft_exec_tree(node->body, fds, i, envp);

	}
}

static int	execute_subshell(t_ast *node, t_fds **fds, int i, char **envp)
{
	int	**new_pipes;
	int	start_point;
	int	end_point;

	start_point = i;
	if (node->cmd)
	{
		apply_redirs_dup(node->cmd, fds);
		end_point = ft_exec_tree(node, fds, i, envp);
		while(start_point < end_point && node->cmd)
		{
			printf("%s", (*fds)->pipe_fds[start_point]);
			start_point++;
		}
	}
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
