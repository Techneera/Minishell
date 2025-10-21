#include "execution.h"

static int	open_files(t_fds **fds, t_ast *ast_root, int i, int r);
static int	create_redir_in(t_fds **fds, t_ast *ast_root, int i, int r);
static int	create_redir_out(t_fds **fds, t_ast *ast_root, int i, int r);

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
				(*fds)->n_files++;
			i++;
		}
	}
}

int	fill_fd_file(t_fds **fds, t_ast *ast_root, int i)
{
	int j;
	int	r;

	r = 0;
	j = 0;
	if (!ast_root)
		return (i);
	i = fill_fd_file(fds, ast_root->left, i);
	i = fill_fd_file(fds, ast_root->right, i);
	if (ast_root->cmd && ast_root->cmd->redirs)
	{
		while (j < ast_root->cmd->redir_count)
		{
			if (open_files(fds, ast_root, i + r, j))
				r++;
			else
				break ;
			j++;
		}
		return (i + r + 1);
	}
	return (i);
}

static int	open_files(t_fds **fds, t_ast *ast_root, int i, int r)
{
	if (ast_root->cmd->redirs[r].label == REDIR_IN)
		return (create_redir_in(fds, ast_root, i, r));
	else if (ast_root->cmd->redirs[r].label != REDIR_HEREDOCK)
		return (create_redir_out(fds, ast_root, i, r));
	return (0);
}

static int	create_redir_in(t_fds **fds, t_ast *ast_root, int i, int r)
{
	if (access(ast_root->cmd->redirs[r].file_name, F_OK) != 0)
	{
		(*fds)->fd_files[i] = -1;
		message_error(": no such file or directory: ",
			ast_root->cmd->redirs[r].file_name, 0);
		return (0);
	}
	else
	{
		(*fds)->fd_files[i] = open(ast_root->cmd->redirs[r].file_name,
				O_RDONLY);
		if ((*fds)->fd_files[i] == -1)
		{
			message_error(": Permission denied",
				ast_root->cmd->redirs->file_name, 0);
			return (0);
		}
	}
	return (1);
}

static int	create_redir_out(t_fds **fds, t_ast *ast_root, int i, int r)
{
	if (ast_root->cmd->redirs[r].label == REDIR_OUT)
		(*fds)->fd_files[i] = open(ast_root->cmd->redirs[r].file_name,
				O_WRONLY | O_CREAT | O_TRUNC, 0671);
	else
		(*fds)->fd_files[i] = open(ast_root->cmd->redirs[r].file_name,
					O_WRONLY | O_CREAT | O_APPEND, 0671);
	if ((*fds)->fd_files[i] == -1)
	{
		message_error(": Permission denied",
			ast_root->cmd->redirs[r].file_name, 0);
		return (0);
	}
		
	return (1);
}