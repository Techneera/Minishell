#include "execution.h"

int	number_of_cmds(t_ast *ast_root);
int execute_pipe(t_ast *node, t_fds **fds, int i, char **envp);
void	create_fds(t_fds **fds, t_ast *ast_root);
int		execute_tree(t_ast *node, t_fds **fds, int i, char **envp);
int	execute_cmd(t_ast *node, t_fds **fds, int i, char **envp);
void	init_pipe(int ***pipe_fds, t_ast *ast_root);

int main(int argc, char *argv[], char **envp)
{
	t_ast	*cmd = ft_cmd5();
	t_fds	*fds;

	fds = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!cmd)
		return (-1);
	create_fds(&fds, cmd);
	execute_tree(cmd, &fds, -1, envp);
	free_all((void **) fds->pipe_fds, number_of_cmds(cmd) - 1);
	free(fds);
	free_tree(cmd);
	//execute_tree(cmd, NULL, envp);
}

void	create_fds(t_fds **fds, t_ast *ast_root)
{
	*fds = malloc(sizeof(t_fds));
	if (!*fds)
		exit(1);
	(*fds)->file_id = 0;
	(*fds)->n_files = 0;
	init_pipe(&((*fds)->pipe_fds), ast_root);
	number_of_redirs(fds, ast_root);
	printf("%d\n", (*fds)->n_files);
}

void	init_pipe(int ***pipe_fds, t_ast *ast_root)
{
	int	n_pipes;
	int	i;

	i = 0;
	n_pipes = number_of_cmds(ast_root) - 1;
	if (n_pipes > 0)
	{
		*pipe_fds = malloc(n_pipes * sizeof(int *));
		if (!*pipe_fds)
			exit(1);
		while (i < n_pipes)
		{
			(*pipe_fds)[i] = malloc(2 * sizeof(int));
			if (!(*pipe_fds)[i])
			{
				free_all((void **) *pipe_fds, n_pipes);
				exit(1);
			}
			if(pipe((*pipe_fds)[i]) == -1)
			{
				free_all((void **) *pipe_fds, n_pipes);
				exit(1);
			}
			i++;
		}
	}
}

int	number_of_cmds(t_ast *ast_root)
{
	int		depth_left;
	int		depth_right;

	if (!ast_root)
		return (0);
	depth_left = number_of_cmds(ast_root->left);
	depth_right = number_of_cmds(ast_root->right);
	if (ast_root->type == NODE_CMD)
		return (1 + depth_left + depth_right);
	else
		return (depth_left + depth_right);
}

int init_pid(pid_t *pid)
{
    *pid = fork();
    if (*pid == -1)
    {
        perror("fork");
        return (0);
    }
    return (1);
}

int	ft_max(int a, int b)
{
	if (a >= b)
		return (a);
	return (b);
}

int	execute_tree(t_ast *node, t_fds **fds, int i, char **envp)
{
	
	(void)envp;
	if (node == NULL)
		return (0);
	if (node->type == NODE_CMD)
	{
		i = execute_cmd(node, fds, i, envp);
		i++;
	}
	else if (node->type == NODE_PIPE)
	{
		i = execute_pipe(node, fds, i, envp);
	}
	return (i);
}

int	execute_cmd(t_ast *node, t_fds **fds, int i, char **envp)
{
	//char    *cmd_path; 
	//char    **args; 
	//pid_t   pid; 
	if (fds)
		(void)envp;

	if (!node)
		return (0);
	if (i == -1)
		i = 0;
	printf("Im in the %d* pipe and gonna execute %s command\n", i, node->cmd->args[0]);
	return (i);
	// args = node->cmd->args;
	// if (!args)
	// 	exit(1);
	// cmd_path = get_command_path(args, envp);
	// if (cmd_path)
	// 	exit(1);
	// if (!init_pid(&pid)) 
	//     exit(1); 
	// if (pid == 0) 
	// { 
	// 	if (!node->cmd->redirs && i > 0)
	// 	{
	// 		if (dup2((*fds)->pipe_fds[i - 1][0], STDIN_FILENO) == -1)
	// 			exit(1);
	// 	}
	// 	if (node->cmd->redirs)
	// 	{
	// 		if (dup2((*fds)->pipe_fds[(*fds)->file_id], STDIN_FILENO) == -1)
	// 			exit(1);
	// 		(*fds)->file_id++;
	// 	}
	// 	if (!node->cmd->redirs && node->cmd->redir_count> 1 && i > 0)
	// 	{
	// 		if (dup2((*fds)->pipe_fds[i][0], STDIN_FILENO) == -1)
	// 			exit(1);
	// 	}
	// 	if (node->cmd->redirs)
	// 	{
	// 		if (dup2((*fds)->pipe_fds[(*fds)->file_id], STDIN_FILENO) == -1)
	// 			exit(1);
	// 		(*fds)->file_id++;
	// 	}
	// 	//ft_closing_all(env); 
	// 	if (execve(cmd_path, args, envp) == -1) 
	// 		exit(1);
	//}
	//return (free_exit(args, cmd_path, 0, NULL), pid);
}

int execute_pipe(t_ast *node, t_fds **fds, int i, char **envp)
{
	(void)envp;
	if (node->left)
		i = execute_tree(node->left, fds, i, envp);
	if (node->right)
		i = execute_tree(node->right, fds, i, envp);
	return (i);
}