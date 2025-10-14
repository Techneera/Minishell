#include "execution.h"

int		number_of_cmds(t_ast *ast_root);
int		execute_pipe(t_ast *node, t_fds **fds, int i, char **envp);
void	create_fds(t_fds **fds, t_ast *ast_root);
int		execute_tree(t_ast *node, t_fds **fds, int i, char **envp);
int		execute_cmd(t_ast *node, t_fds **fds, int i, char **envp);
void	init_pipe(t_fds **fds, int ***pipe_fds, t_ast *ast_root);
void	ft_closing_all(t_fds **fds);

int main(int argc, char *argv[], char **envp)
{
	t_ast	*cmd = ft_cmd9();
	t_fds	*fds;

	fds = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!cmd)
		return (-1);
	create_fds(&fds, cmd);
	execute_tree(cmd, &fds, -1, envp);
	ft_closing_all(&fds); 
	while (waitpid(-1, NULL, 0) > 0)
		;
	free_all((void **) fds->pipe_fds, number_of_cmds(cmd) - 1);
	free(fds);
	//free_tree(cmd);
}

void	create_fds(t_fds **fds, t_ast *ast_root)
{
	*fds = malloc(sizeof(t_fds));
	if (!*fds)
		exit(1);
	(*fds)->file_id = 0;
	init_pipe(fds, &((*fds)->pipe_fds), ast_root);
	number_of_redirs(fds, ast_root);
	if ((*fds)->n_files > 0)
	{
		(*fds)->fd_files = malloc((*fds)->n_files * sizeof(int));
		if (!(*fds)->fd_files)
			exit(1);
	}
	fill_fd_file(fds, ast_root, 0);
}

void	init_pipe(t_fds **fds, int ***pipe_fds, t_ast *ast_root)
{
	int	n_pipes;
	int	i;

	i = 0;
	n_pipes = number_of_cmds(ast_root) - 1;
	(*fds)->n_pipes = n_pipes;
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

int	execute_tree(t_ast *node, t_fds **fds, int i, char **envp)
{
	
	(void)envp;
	if (node == NULL)
		return (0);
	if (node->type == NODE_CMD)
	{
		i = execute_cmd(node, fds, i, envp);
		(*fds)->file_id += node->cmd->redir_count;
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
	char	*cmd_path;
	char	**args;
	pid_t	pid;
	int fd_heredoc;

	if (!node)
		return (i);
	if (i == -1)
		i = 0;
	args = node->cmd->args;
	if (!args)
		exit(1);
	cmd_path = get_command_path(args, envp);
	if (!cmd_path)
		exit(1);
	if (!init_pid(&pid))
	    exit(1);
	if (pid == 0)
	{
		int r = 0;

		if (i > 0 && (*fds)->n_pipes > 0)
		{
			if (dup2((*fds)->pipe_fds[i - 1][0], STDIN_FILENO) == -1)
				exit(1);
		}
		if (i < (*fds)->n_pipes)
		{
			if (dup2((*fds)->pipe_fds[i][1], STDOUT_FILENO) == -1)
				exit(1);
		}
		while (r < node->cmd->redir_count)
		{
			if (node->cmd->redirs[r].label == REDIR_HEREDOCK)
			{
				fd_heredoc = here_doc(node->cmd->redirs[r].file_name);
				if (dup2(fd_heredoc, STDIN_FILENO) == -1)
					exit(1);
				if (fd_heredoc != -1)
					close(fd_heredoc);
			}
			if (node->cmd->redirs[r].label == REDIR_IN)
			{
				if (dup2((*fds)->fd_files[(*fds)->file_id], STDIN_FILENO) == -1)
					exit(1);
				(*fds)->file_id++;
			}
			else if (node->cmd->redirs[r].label == REDIR_OUT
				|| node->cmd->redirs[r].label == REDIR_APPEND)
			{
				if (dup2((*fds)->fd_files[(*fds)->file_id], STDOUT_FILENO) == -1)
					exit(1);
				(*fds)->file_id++;
			}
			r++;
		}
		ft_closing_all(fds);
		execve(cmd_path, args, envp);
		perror("execve");
		exit(1);
	}
	free_all((void **)args, ft_arraylen((void **)args));
	free(cmd_path);
	return (i);
}

void ft_closing_all(t_fds **fds)
{
	int	i;
	if (!fds || !*fds)
		return ;	
	i = 0;
	while (i < (*fds)->n_pipes)
	{
		if ((*fds)->pipe_fds && (*fds)->pipe_fds[i])
		{
			if ((*fds)->pipe_fds[i][0] != -1)
			{
				close((*fds)->pipe_fds[i][0]);
				(*fds)->pipe_fds[i][0] = -1;
			}
			if ((*fds)->pipe_fds[i][1] != -1)
			{
				close((*fds)->pipe_fds[i][1]);
				(*fds)->pipe_fds[i][1] = -1;
			}
		}
		i++;
	}
	i = 0;
	while (i < (*fds)->n_files)
	{
		if ((*fds)->fd_files) {
			if ((*fds)->fd_files[i] != -1)
			{
				close((*fds)->fd_files[i]);
				(*fds)->fd_files[i] = -1;
			}
		}
		i++;
	}
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
