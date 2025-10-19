#include "execution.h"

void	free_all(void **ptr, size_t rows)
{
	size_t	i;

	i = 0;
	if (!ptr)
		return ;
	while (i < rows)
	{
		if (ptr[i])
		{
			free(ptr[i]);
			ptr[i] = NULL;
			i++;
		}
	}
	free(ptr);
	ptr = NULL;
}

int	ft_arraylen(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (i);
	while (ptr[i])
		i++;
	return (i);
}

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
				free(ast_root->cmd->redirs[ast_root->cmd->redir_count].file_name);
		}
		free(ast_root->cmd->redirs);
		free(ast_root->cmd);
	}
	free(ast_root);
}
