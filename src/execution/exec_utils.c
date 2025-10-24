#include "execution.h"

static void	create_pipe(t_fds **fds, int ***pipe_fds);

void	free_tree(t_ast *ast_root)
{
	if (!ast_root)
		return ;
	free_tree(ast_root->left);
	free_tree(ast_root->right);
	if (ast_root->cmd)
	{
		free_all((void **) ast_root->cmd->args,
			ft_arraylen((void **) ast_root->cmd->args));
		while (ast_root->cmd->redir_count-- > 0)
		{
			if (ast_root->cmd->redirs[ast_root->cmd->redir_count].file_name)
				free(ast_root->cmd
					->redirs[ast_root->cmd->redir_count].file_name);
		}
		free(ast_root->cmd->redirs);
		free(ast_root->cmd);
	}
	free(ast_root);
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
	(*fds)->c_pids = ft_calloc((*fds)->n_pipes + 2, sizeof(int));
	if (!(*fds)->c_pids)
		exit(1);
	init_heredoc(fds, ast_root);
	fill_fd_file(fds, ast_root, 0);
}

void	init_pipe(t_fds **fds, int ***pipe_fds, t_ast *ast_root)
{
	int	n_pipes;

	n_pipes = number_of_cmds(ast_root) - 1;
	(*fds)->n_pipes = n_pipes;
	if (n_pipes > 0)
	{
		*pipe_fds = malloc(n_pipes * sizeof(int *));
		if (!*pipe_fds)
			exit(1);
		create_pipe(fds, pipe_fds);
	}
}

static void	create_pipe(t_fds **fds, int ***pipe_fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->n_pipes)
	{
		(*pipe_fds)[i] = malloc(2 * sizeof(int));
		if (!(*pipe_fds)[i])
		{
			free_all((void **) *pipe_fds, (*fds)->n_pipes);
			exit(1);
		}
		if (pipe((*pipe_fds)[i]) == -1)
		{
			free_all((void **) *pipe_fds, (*fds)->n_pipes);
			exit(1);
		}
		i++;
	}
}
