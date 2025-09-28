#include "execution.h"

int	number_of_cmds(t_ast *ast_root);
int	execute_tree(t_ast *node, int fd_in[2], char **envp);
void	init_pipe(int **pipe_fds, t_ast *ast_root);
//void	execute_cmd(int *pipe_fd, int pos);

int main(int argc, char *argv[], char **envp)
{
	t_ast	*cmd = ft_cmd2();
	int		*pipe_fds;

	if (!cmd)
		return (-1);
	init_pipe(&pipe_fds, cmd);
	execute_tree(cmd, NULL, envp);
}

void	init_pipe(int **pipe_fds, t_ast *ast_root)
{
	int	n_pipes;
	int	i;

	i = -1;
	n_pipes = number_of_cmds(ast_root) - 1;
	if (n_pipes > 0)
	{
		*pipe_fds = malloc(n_pipes * sizeof(int));
		if (!*pipe_fds)
			exit(1);
	}
	else
	{
		while (i < n_pipes)
		{
			if(pipe(pipe_fds[++i]) == -1)
			{
				free(pipe_fds);
				exit(1);
			}
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

int	execute_tree(t_ast *node, int fd_in[2], char **envp)
{
	int	fd_out[2];

	if (pipte(fd_out) == -1)
		exit(1);
	if (node == NULL)
		return (0);	
	if (node->type == NODE_CMD)
	{
		return (execute_cmd(node->cmd, fd_in[2], fd_out[2], envp));
	}
	else if (node->type == NODE_PIPE)
	{
		return (execute_pipe(node, envp));
	}
	return (0);
}

int	execute_cmd(t_ast *node, char **envp, int fd_in[2], int fd_out[2])
{
	char    *cmd_path; 
	char    **args; 
	pid_t   pid; 

	if (!node)
		return (0);
	args = node->cmd->args;
	if (!args)
		exit(1);
	cmd_path = get_command_path(args, envp);
	if (!init_pid(&pid)) 
	    exit(1); 
	if (pid == 0) 
	{ 
		if (!cmd_path || !(*args)) 
			exit(1); 
		if (node->cmd->redirs && node->cmd->redirs->label == REDIR_IN)
		{
			
		}
		if (dup2(fd_out[1], STDOUT_FILENO) == -1) 
			exit(1);
		//ft_closing_all(env); 
		if (execve(cmd_path, args, envp) == -1) 
			exit(1);
	}
	return (free_exit(args, cmd_path, 0, NULL), pid);
}

int execute_pipe(t_ast *node, char **envp)
{
	int pipe_fds[2];
	int status;

	if (pipe(pipe_fds) == -1)
		return (-1);
	pid_t pid_left = fork();
	if (pid_left == 0)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		exit(execute_tree(node->left, pipe_fds, envp));
	}
	pid_t pid_right = fork();
	if (pid_right == 0)
	{
		close(pipe_fds[1]); 
		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);
		exit(execute_tree(node->right, pipe_fds, envp));
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(pid_right, &status, 0);
	waitpid(pid_left, &status, 0);
	return (status);
}