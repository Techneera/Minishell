#include "execution.h"

static void	create_files(t_data *data);
static void	get_sizes_bonus(t_ast *ast_root, t_fds **fds, int inside_sshell);

void	ft_create_fds_bonus(t_data *data)
{
	t_fds	*fds;
	t_ast	*ast_root;

	if (!data->tree)
		return ;
	ast_root = data->tree;
	fds = data->fds;
	fds->get.n_files = 0;
	fds->get.n_cmds = 0;
	get_sizes_bonus(ast_root, &fds, 0);
	if (fds->get.n_cmds > 0)
	{
		fds->c_pids = ft_calloc(fds->get.n_cmds, sizeof(int));
		if (!fds->c_pids)
			secure_exit(data, FAIL_STATUS);
	}
	create_files(data);
}

static void	get_sizes_bonus(t_ast *ast_root, t_fds **fds, int inside_sshell)
{
	int	i;

	if (!ast_root)
		return ;
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
		return ;
	i = 0;
	if (ast_root->type == NODE_SUBSHELL)
		get_sizes_bonus(ast_root->body, fds, 1);
	get_sizes_bonus(ast_root->left, fds, inside_sshell);
	get_sizes_bonus(ast_root->right, fds, inside_sshell);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		if (!inside_sshell)
			(*fds)->get.n_cmds++;
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label != REDIR_HEREDOCK)
				(*fds)->get.n_files++;
			i++;
		}
	}
}

static void	create_files(t_data *data)
{
	t_fds	*fds;
	int		i;

	i = 0;
	fds = data->fds;
	if (fds->get.n_files > 0)
	{
		fds->fd_files = ft_calloc(fds->get.n_files, sizeof(int));
		if (!fds->fd_files)
			secure_exit(data, FAIL_STATUS);
		while (++i < fds->get.n_files)		
			fds->fd_files[i] = -1;
	}
	fill_fd_file(data, data->tree, 0);	
}
