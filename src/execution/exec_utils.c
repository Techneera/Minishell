#include "execution.h"

void	free_all(void **ptr, size_t rows)
{
	size_t	i;

	i = 0;
	if (!*ptr)
		return ;
	while (i  < rows)
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

void	free_tree(t_ast *ast_root)
{
	if (!ast_root)
		return ;
	free_tree(ast_root->left);
	free_tree(ast_root->right);
	if (ast_root->cmd)
		free_all((void **) ast_root->cmd->args, ft_strlen(ast_root->cmd->args));
	free(ast_root);
}