#include "execution.h"
static void heredoc_subshell(t_cmd *cmd, t_fds **fds, int r);

void	apply_redirs_subshell(t_data *data)
{
	int		r;
	t_fds	*fds;
	t_cmd	*cmd;

	r = -1;
	fds = data->fds;
	cmd = data->tree->cmd;
	while (++r < cmd->redir_count)
	{
		heredoc_subshell(cmd, &fds, r);
		if (cmd->redirs[r].label == REDIR_IN)
		{
			if (dup2(fds->fd_files[fds->pos.file_id], STDIN_FILENO) == -1)
				secure_exit(data, 1);
			fds->pos.file_id++;
		}
		else if (cmd->redirs[r].label == REDIR_OUT
			|| cmd->redirs[r].label == REDIR_APPEND)
		{
			if (fds->fd_files[fds->pos.file_id] != -1
				&& dup2(fds->fd_files[fds->pos.file_id], STDOUT_FILENO) == -1)
				secure_exit(data, 1);
			fds->pos.file_id++;
		}
	}
}

static void	heredoc_subshell(t_cmd *cmd, t_fds **fds, int r)
{
	if (cmd->redirs[r].label == REDIR_HEREDOCK)
	{
		dup2((*fds)->heredoc_fds[(*fds)->pos.doc_id][0], STDIN_FILENO);
		(*fds)->pos.doc_id++;
	}
}