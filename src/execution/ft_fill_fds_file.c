#include "execution.h"

static int	open_files(t_data *data, t_ast *ast_root, int i, int r);
static int	create_redir_in(t_fds **fds, t_ast *ast_root, int i, int r);
static int	create_redir_out(t_fds **fds, t_ast *ast_root, int i, int r);
static int	fill_files(t_data *data, t_ast *ast_root, int i);

int	fill_fd_file(t_data *data, t_ast *ast_root, int i)
{
	if (!ast_root)
		return (i);
	if (ast_root->type == NODE_AND || ast_root -> type == NODE_OR)
		return (i);
	if (ast_root->type == NODE_SUBSHELL)
		i = fill_files(data, ast_root, i);
	i = fill_fd_file(data, ast_root->left, i);
	i = fill_fd_file(data, ast_root->right, i);
	i = fill_fd_file(data, ast_root->body, i);
	if (ast_root->type == NODE_CMD)
		return (fill_files(data, ast_root, i));
	return (i);
}

static int	fill_files(t_data *data, t_ast *ast_root, int i)
{
	int		j;
	int		r;

	r = 0;
	j = 0;
	if (ast_root->cmd->redirs)
	{
		while (j < ast_root->cmd->redir_count)
		{
			if (open_files(data, ast_root, i + r, j))
			{
				if (ast_root->cmd->redirs[j].label != REDIR_HEREDOCK)
					r++;
			}
			else
			{
				r++;
				break ;
			}
			j++;
		}
		return (i + r);
	}
	return (i);
}

static int	open_files(t_data *data, t_ast *ast_root, int i, int r)
{
	t_fds	*fds;

	fds = data->fds;
	if (ast_root->cmd->redirs[r].label != REDIR_HEREDOCK)
		ast_root->cmd->redirs[r].file_name = \
expand_word(ast_root->cmd->redirs[r].file_name, \
data->envp, ft_exit_status(0, 0, 0));
	if (ast_root->cmd->redirs[r].label == REDIR_IN)
		return (create_redir_in(&fds, ast_root, i, r));
	else if (ast_root->cmd->redirs[r].label != REDIR_HEREDOCK)
		return (create_redir_out(&fds, ast_root, i, r));
	return (1);
}

static int	create_redir_in(t_fds **fds, t_ast *ast_root, int i, int r)
{
	if (access(ast_root->cmd->redirs[r].file_name, F_OK) != 0)
	{
		(*fds)->fd_files[i] = -1;
		message_error(": No such file or directory",
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
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		(*fds)->fd_files[i] = open(ast_root->cmd->redirs[r].file_name,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*fds)->fd_files[i] == -1)
	{
		message_error(": Permission denied",
			ast_root->cmd->redirs[r].file_name, 0);
		return (0);
	}
	return (1);
}
