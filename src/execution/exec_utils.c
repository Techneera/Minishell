#include "execution.h"

void secure_exit(t_data *data, int status)
{
	free_tree(&data->root);
	ft_closing_all(&data->fds);
	free_fds(&data->fds);
	ft_lstclear(&data->env_list, &free);
	exit(status);
}

void free_data(t_data *data)
{
	free_tree(&data->root);
	ft_closing_all(&data->fds);
	free_fds(&data->fds);
	ft_lstclear(&data->env_list, &free);
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

int	docs_bonus(t_ast *ast_root, t_fds **fds)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!ast_root)
		return (i);
	i += docs_bonus(ast_root->body, fds);
	i += docs_bonus(ast_root->left, fds);
	i += docs_bonus(ast_root->right, fds);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		while (j < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[j].label == REDIR_HEREDOCK)
				i++;
			j++;
		}
	}
	return (i);
}

int	is_builtin(t_data *data, char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "echo\0", 5) == 0)
		return (ft_echo(data), 1);
	if (ft_strncmp(arg, "cd\0", 3) == 0)
		return (ft_cd(data->tree), 1);
	if (ft_strncmp(arg, "pwd\0", 4) == 0)
		return (ft_pwd(), 1);
	if (ft_strncmp(arg, "export\0", 7) == 0)
		return (ft_export(data->env_list, data->tree, data), 1);
	if (ft_strncmp(arg, "unset\0", 6) == 0)
		return (0);
	if (ft_strncmp(arg, "env\0", 4) == 0)
		return (ft_env(data), 1);
	if (ft_strncmp(arg, "exit\0", 5) == 0)
		return (0);
	return (0);
}

void	get_sizes(t_ast *ast_root, t_fds **fds, int inside_sshell)
{
	int	i;

	if (!ast_root)
		return ;
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
	{
		(*fds)->get.n_docs += docs_bonus(ast_root, fds);
		return ;
	}
	i = 0;
	if (ast_root->type == NODE_SUBSHELL)
		get_sizes(ast_root->body, fds, 1);
	get_sizes(ast_root->left, fds, inside_sshell);
	get_sizes(ast_root->right, fds, inside_sshell);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		if (!inside_sshell)
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
}

