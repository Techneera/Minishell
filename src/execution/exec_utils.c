#include "execution.h"

void	secure_exit(t_data *data, int status)
{
	free_tree(&data->root);
	data->tree = NULL;
	ft_closing_all(&data->fds);
	free_fds(&data->fds);
	ft_lstclear(&data->env_list, &ft_free_content);
	ft_free_array(data->envp);
	free_lexer(data->lexer);
	free(data->rl);
	data->rl = NULL;
	exit(status);
}

void	free_data(t_data *data)
{
	free_tree(&data->root);
	ft_closing_all(&data->fds);
	free_fds(&data->fds);
	free_lexer(data->lexer);
	data->lexer = NULL;
	free(data->rl);
	data->rl = NULL;
	data->tree = NULL;
}

void	free_tree(t_ast **ast_root)
{
	if (!(*ast_root))
		return ;
	free_tree(&(*ast_root)->body);
	free_tree(&(*ast_root)->left);
	free_tree(&(*ast_root)->right);
	if ((*ast_root)->cmd)
	{
		free_all((void **)(*ast_root)->cmd->args,
			ft_arraylen((void **)(*ast_root)->cmd->args));
		while ((*ast_root)->cmd->redir_count-- > 0)
			if ((*ast_root)->cmd->redirs[(
						*ast_root)->cmd->redir_count].file_name)
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
	if ((*fds)->heredoc_fds)
		free_all((void **)(*fds)->heredoc_fds, (*fds)->get.n_docs);
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
