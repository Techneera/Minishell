#include "execution.h"

void secure_exit(t_data *data, int status)
{
	free_tree(&data->root);
	ft_closing_all(&data->fds);
	free_fds(&data->fds);
	exit(status);
}

void	free_tree(t_ast **ast_root)
{
	if (!(*ast_root))
		return ;
	free_tree(&(*ast_root)->left);
	free_tree(&(*ast_root)->right);
	if ((*ast_root)->cmd)
	{
		free_all((void **) (*ast_root)->cmd->args,
			ft_arraylen((void **) (*ast_root)->cmd->args));
		while ((*ast_root)->cmd->redir_count-- > 0)
			if ((*ast_root)->cmd->redirs[(*ast_root)->cmd->redir_count].file_name)
				free((*ast_root)->cmd
					->redirs[(*ast_root)->cmd->redir_count].file_name);
		free((*ast_root)->cmd->redirs);
		free((*ast_root)->cmd);
		(*ast_root)->cmd = NULL;
	}
	(*ast_root)->left = NULL;
	(*ast_root)->right = NULL;
	free((*ast_root));
	(*ast_root) = NULL;
}

void	free_fds(t_fds **fds)
{
	if (!(*fds))
		return ;
	if ((*fds)->pipe_fds)
		free_all((void **) (*fds)->pipe_fds, (*fds)->get.n_pipes);
	if ((*fds)->heredoc_fds)
		free_all((void **) (*fds)->heredoc_fds, (*fds)->get.n_docs);
	if ((*fds)->fd_files)
		free((*fds)->fd_files);
	if ((*fds)->c_pids)
		free((*fds)->c_pids);
	free((*fds));
	(*fds) = NULL;
}

int	number_of_cmds(t_ast *ast_root)
{
	int		depth_left;
	int		depth_right;

	depth_left = 0;
	depth_right = 0;
	if (!ast_root)
		return (0);
	depth_left = number_of_cmds(ast_root->left);
	depth_right = number_of_cmds(ast_root->right);
	if (ast_root->type == NODE_CMD)
		return (1 + depth_left + depth_right);
	else
		return (depth_left + depth_right);
}

void	get_sizes(t_ast *ast_root, t_fds **fds)
{
	int	i;

	if (!ast_root)
		return ;
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
		return ;
	i = 0;
	get_sizes(ast_root->body, fds);
	get_sizes(ast_root->left, fds);
	get_sizes(ast_root->right, fds);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		if (ast_root->type == NODE_CMD)
			(*fds)->get.n_cmds++;
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label != REDIR_HEREDOCK)
				(*fds)->get.n_files++;
			if (ast_root->cmd->redirs[i].label == REDIR_HEREDOCK)
				(*fds)->get.n_docs++;
			i++;
		}
	}
	else if (ast_root->type == NODE_PIPE)
		(*fds)->get.n_pipes++;
}
