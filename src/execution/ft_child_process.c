#include "execution.h"

static void	apply_std_dup(t_fds **fds, int i);
static void	apply_redirs_dup(t_ast *node, t_fds **fds);
static void	heredoc_dup(t_ast *node, t_fds **fds, int r);

void	ft_child_process(t_ast *node, t_fds **fds, int i, char **envp)
{
	char	*cmd_path;
	char	**args;	

	args = node->cmd->args;
	cmd_path = get_command_path(args, envp);
	apply_std_dup(fds, i);
	apply_redirs_dup(node, fds);
	ft_closing_all(fds);
	execve(cmd_path, args, envp);
	perror("execve");
	if (!cmd_path)
	{
		free(cmd_path);
		exit(CMD_NOT_FOUND);
	}
	exit(1);
}

static void	apply_std_dup(t_fds **fds, int i)
{
	if (i > 0 && (*fds)->get.n_pipes > 0)
	{
		if (dup2((*fds)->pipe_fds[i - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (i < (*fds)->get.n_pipes)
	{
		if (dup2((*fds)->pipe_fds[i][1], STDOUT_FILENO) == -1)
			exit(1);
	}
}

static void	apply_redirs_dup(t_ast *node, t_fds **fds)
{
	int	r;

	r = 0;
	while (r < node->cmd->redir_count)
	{
		heredoc_dup(node, fds, r);
		if (node->cmd->redirs[r].label == REDIR_IN)
		{
			if (dup2((*fds)->fd_files[(*fds)->pos.file_id], STDIN_FILENO) == -1)
				exit(1);
			(*fds)->pos.file_id++;
		}
		else if (node->cmd->redirs[r].label == REDIR_OUT
			|| node->cmd->redirs[r].label == REDIR_APPEND)
		{
			if ((*fds)->fd_files[(*fds)->pos.file_id] != -1
				&& dup2((*fds)->fd_files[(*fds)->pos.file_id], STDOUT_FILENO) == -1)
				perror("dup on REDIR_OUT");
			(*fds)->pos.file_id++;
		}
		r++;
	}
}

static void	heredoc_dup(t_ast *node, t_fds **fds, int r)
{
	if (node->cmd->redirs[r].label == REDIR_HEREDOCK)
	{
		if ((*fds)->heredoc_fds[(*fds)->pos.doc_id][0] != -1
		&& dup2((*fds)->heredoc_fds[(*fds)->pos.doc_id][0], STDIN_FILENO) == -1)
			perror("dup on REDIR_DOC");
		(*fds)->pos.doc_id++;
	}
}
