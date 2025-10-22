#include "execution.h"

int		number_of_cmds(t_ast *ast_root);
int		number_of_pipes(t_ast *ast_root);
int		execute_pipe(t_ast *node, t_fds **fds, int i, char **envp);
void	create_fds(t_fds **fds, t_ast *ast_root);
int		execute_tree(t_ast *node, t_fds **fds, int i, char **envp);
int		execute_cmd(t_ast *node, t_fds **fds, int i, char **envp);
void	init_pipe(t_fds **fds, int ***pipe_fds, t_ast *ast_root);
void	ft_closing_all(t_fds **fds);
void	init_heredoc(t_fds **fds, t_ast *node);
void	fill_heredoc(t_fds **fds, t_ast *node, int i);
int		number_of_heredocks(t_ast *ast_root);

void	init_heredoc(t_fds **fds, t_ast *node)
{
	int	n_pipes;
	int	i;

	i = 0;
	n_pipes = number_of_heredocks(node);
	(*fds)->n_docs = n_pipes;
	(*fds)->doc_id = 0;
	(*fds)->heredoc_fds = NULL;
	if (n_pipes > 0)
	{
		(*fds)->heredoc_fds = malloc(n_pipes * sizeof(int *));
		if (!(*fds)->heredoc_fds)
			exit(1);
		while (i < n_pipes)
		{
			(*fds)->heredoc_fds[i] = malloc(2 * sizeof(int));
			if (!(*fds)->heredoc_fds[i])
			{
				free_all((void **) (*fds)->heredoc_fds, n_pipes);
				exit(1);
			}
			if(pipe((*fds)->heredoc_fds[i]) == -1)
			{
				free_all((void **) (*fds)->heredoc_fds, n_pipes);
				exit(1);
			}
			i++;
		}
		fill_heredoc(fds, node, 0);
	}
}

void	fill_heredoc(t_fds **fds, t_ast *node, int i)
{
	int	y;
	int	r;

	y = 0;
	r = 0;
	if (node->left)
		fill_heredoc(fds, node->left, i);
	if (node->right)
		fill_heredoc(fds, node->right, i);
	if (!node->cmd && !node->cmd->redirs)
		return ;
	i++;
	while (y < node->cmd->redir_count)
	{
		if (node->cmd->redirs[y].label == REDIR_HEREDOCK)
		{
			here_doc(node->cmd->redirs[y].file_name, (*fds)->heredoc_fds[r]);
			r++;
		}
		y++;
	}
}

void	create_fds(t_fds **fds, t_ast *ast_root)
{
	*fds = malloc(sizeof(t_fds));
	(*fds)->n_files = 0;
	(*fds)->pipe_fds = 0;
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
	init_heredoc(fds, ast_root);
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

int	number_of_heredocks(t_ast *ast_root)
{
	int		depth_left;
	int		depth_right;
	int		i;
	int		n;

	i = 0;
	n = 0;
	if (!ast_root)
		return (0);
	depth_left = number_of_heredocks(ast_root->left);
	depth_right = number_of_heredocks(ast_root->right);
	if (ast_root->cmd)
	{
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label == REDIR_HEREDOCK)
				n++;
			i++;
		}
		return (n + depth_left + depth_right);
	}
	else
		return (depth_left + depth_right);
}

int	number_of_pipes(t_ast *ast_root)
{
	int		depth_left;
	int		depth_right;
	int		i;


	i = 1;
	if (!ast_root)
		return (0);
	depth_left = number_of_cmds(ast_root->left);
	depth_right = number_of_cmds(ast_root->right);
	if (ast_root->type == NODE_CMD)
	{
		if (ast_root->cmd && ast_root->cmd->redirs &&
				ast_root->cmd->redirs->label == REDIR_HEREDOCK)
			i++;
		return (i + depth_left + depth_right);
	}
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

	if (!node)
		return (i);
	if (i == -1)
		i = 0;
	args = node->cmd->args;
	if (!args)
		exit(1);
	cmd_path = get_command_path(args, envp);
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
				if ((*fds)->heredoc_fds[(*fds)->doc_id][0] != -1 && 
					dup2((*fds)->heredoc_fds[(*fds)->doc_id][0], STDIN_FILENO) == -1)
					perror("dup on REDIR_DOC");
				(*fds)->doc_id++;
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
				if ((*fds)->fd_files[(*fds)->file_id] != -1 && 
				dup2((*fds)->fd_files[(*fds)->file_id], STDOUT_FILENO) == -1)
					perror("dup on REDIR_OUT");
				(*fds)->file_id++;
			}
			r++;
		}
		ft_closing_all(fds);
		if (!cmd_path)
			exit(CMD_NOT_FOUND);
		execve(cmd_path, args, envp);
		perror("execve");
		exit(1);
	}
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
	while (i < (*fds)->n_docs)
	{
		if ((*fds)->heredoc_fds && (*fds)->heredoc_fds[i])
		{
			if ((*fds)->heredoc_fds[i][0] != -1)
			{
				close((*fds)->heredoc_fds[i][0]);
				(*fds)->heredoc_fds[i][0] = -1;
			}
			if ((*fds)->heredoc_fds[i][1] != -1)
			{
				close((*fds)->heredoc_fds[i][1]);
				(*fds)->heredoc_fds[i][1] = -1;
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
