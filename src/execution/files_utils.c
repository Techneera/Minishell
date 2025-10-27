#include "execution.h"

void	number_of_redirs(t_fds **fds, t_ast *ast_root)
{
	int	i;

	i = 0;
	if (!ast_root)
		return ;
	number_of_redirs(fds, ast_root->left);
	number_of_redirs(fds, ast_root->right);
	if (ast_root->type == NODE_CMD)
	{
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label != REDIR_HEREDOCK)
				(*fds)->get.n_files++;
			i++;
		}
	}
}
