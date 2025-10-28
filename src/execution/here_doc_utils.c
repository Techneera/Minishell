#include "execution.h"

static int	number_of_heredocks(t_ast *ast_root);
static void	malloc_heredoc(t_fds **fds);
static void	fill_heredoc(t_fds **fds, t_ast *node, int i);

void	init_heredoc(t_fds **fds, t_ast *node)
{
	int	n_pipes;

	n_pipes = number_of_heredocks(node);
	(*fds)->n_docs = n_pipes;
	(*fds)->doc_id = 0;
	(*fds)->heredoc_fds = NULL;
	if (n_pipes > 0)
	{
		(*fds)->heredoc_fds = malloc(n_pipes * sizeof(int *));
		if (!(*fds)->heredoc_fds)
			exit(1);
		malloc_heredoc(fds);
		fill_heredoc(fds, node, 0);
	}
}

static int	number_of_heredocks(t_ast *ast_root)
{
	int		depth_left;
	int		depth_right;
	int		i;
	int		n;

	i = 0;
	n = 0;
	if (!ast_root)
		return (0);
	depth_left = number_of_heredocks(ast_root->left);
	depth_right = number_of_heredocks(ast_root->right);
	if (ast_root->cmd)
	{
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label == REDIR_HEREDOCK)
				n++;
			i++;
		}
		return (n + depth_left + depth_right);
	}
	else
		return (depth_left + depth_right);
}

static void	malloc_heredoc(t_fds **fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->n_docs)
	{
		(*fds)->heredoc_fds[i] = malloc(2 * sizeof(int));
		if (!(*fds)->heredoc_fds[i])
		{
			free_all((void **)(*fds)->heredoc_fds, (*fds)->n_docs);
			exit(1);
		}
		if (pipe((*fds)->heredoc_fds[i]) == -1)
		{
			free_all((void **)(*fds)->heredoc_fds, (*fds)->n_docs);
			exit(1);
		}
		i++;
	}
}

static void	fill_heredoc(t_fds **fds, t_ast *node, int i)
{
	int	y;
	int	r;

	y = 0;
	r = 0;
	if (node->left)
		fill_heredoc(fds, node->left, i);
	if (node->right)
		fill_heredoc(fds, node->right, i);
	if (!node->cmd && !node->cmd->redirs)
		return ;
	i++;
	while (y < node->cmd->redir_count)
	{
		if (node->cmd->redirs[y].label == REDIR_HEREDOCK)
		{
			here_doc(node->cmd->redirs[y].file_name, (*fds)->heredoc_fds[r]);
			r++;
		}
		y++;
	}
}
