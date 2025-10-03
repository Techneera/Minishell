#include "execution.h"

void	number_of_redirs(t_fds **fds, t_ast *ast_root)
{
	if (!ast_root)
		return ;
	number_of_redirs(fds, ast_root->left);
	number_of_redirs(fds, ast_root->right);
	if (ast_root->type == NODE_CMD)
		(*fds)->n_files += ast_root->cmd->redir_count;
}