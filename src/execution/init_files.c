#include "execution.h"

static void	open_files(t_fds **fds, t_ast *ast_root, int i);

void	number_of_redirs(t_fds **fds, t_ast *ast_root)
{
	if (!ast_root)
		return ;
	number_of_redirs(fds, ast_root->left);
	number_of_redirs(fds, ast_root->right);
	if (ast_root->type == NODE_CMD)
		(*fds)->n_files += ast_root->cmd->redir_count;
}

int	fill_fd_file(t_fds **fds, t_ast *ast_root, int i)
{
	if (!ast_root)
		return (i);
	i = fill_fd_file(fds, ast_root->left, i);
	i = fill_fd_file(fds, ast_root->right, i);
	if (ast_root->cmd && ast_root->cmd->redirs)
	{
		open_files(fds, ast_root, i);
		return (i + 1);
	}
	return (i);
}

static void	open_files(t_fds **fds, t_ast *ast_root, int i)
{
	if (ast_root->cmd->redirs->label == REDIR_IN)
	{
		if (access(ast_root->cmd->redirs->file_name, F_OK) != 0)
		{
			(*fds)->fd_files[i] = -1;
			message_error(": no such file or directory: ",
				ast_root->cmd->redirs->file_name, 0);
		}
		else
		{
			(*fds)->fd_files[i] = open(ast_root->cmd->redirs->file_name,
					O_RDONLY);
			if ((*fds)->fd_files[i] == -1)
				message_error(": Permission denied",
					ast_root->cmd->redirs->file_name, 0);
		}
	}
	else
	{
		(*fds)->fd_files[i] = open(ast_root->cmd->redirs->file_name,
				O_WRONLY | O_CREAT | O_TRUNC, 0671);
		if ((*fds)->fd_files[i] == -1)
			message_error(": Permission denied",
				ast_root->cmd->redirs->file_name, 0);
	}
}
