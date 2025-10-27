#include "execution.h"

static void	malloc_heredoc(t_fds **fds);
static void	fill_heredoc(t_fds **fds, t_ast *node, int i);

void	init_heredoc(t_fds **fds, t_ast *node)
{
	(*fds)->pos.doc_id = 0;
	(*fds)->heredoc_fds = NULL;
	if ((*fds)->get.n_docs > 0)
	{
		(*fds)->heredoc_fds = malloc((*fds)->get.n_pipes * sizeof(int *));
		if (!(*fds)->heredoc_fds)
			exit(1);
		malloc_heredoc(fds);
		fill_heredoc(fds, node, 0);
	}
}

static void	malloc_heredoc(t_fds **fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->get.n_docs)
	{
		(*fds)->heredoc_fds[i] = malloc(2 * sizeof(int));
		if (!(*fds)->heredoc_fds[i])
		{
			free_all((void **)(*fds)->heredoc_fds, (*fds)->get.n_docs);
			exit(1);
		}
		if (pipe((*fds)->heredoc_fds[i]) == -1)
		{
			free_all((void **)(*fds)->heredoc_fds, (*fds)->get.n_docs);
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
