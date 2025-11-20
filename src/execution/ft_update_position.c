#include "execution.h"

void	update_positions(t_data *data)
{
	t_fds	*fds;
	t_ast	*node;
	int		j;

	j = 0;
	fds = data->fds;
	node = data->tree;
	while (j < node->cmd->redir_count)
	{
		if (node->cmd->redirs[j].label != REDIR_HEREDOCK)
		{
			secure_close(&fds->fd_files[fds->pos.file_id]);
			fds->pos.file_id++;
		}
		else
		{
			secure_close(&fds->heredoc_fds[fds->pos.doc_id][0]);
			secure_close(&fds->heredoc_fds[fds->pos.doc_id][1]);
			fds->pos.doc_id++;
		}
		j++;
	}
}

