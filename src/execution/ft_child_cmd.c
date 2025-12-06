/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static void	apply_redirs_dup(t_data *data, t_ast **node);
static void	heredoc_dup(t_data *data, int r);
static void	file_dup(t_data *data, t_ast **node, int r);

void	ft_child_cmd(t_data	*data, char **envp)
{
	char	*cmd_path;
	char	**args;	
	t_fds	*fds;
	t_ast	*node;

	fds = data->fds;
	node = data->tree;
	apply_redirs_dup(data, &node);
	args = node->cmd->args;
	cmd_path = get_command_path(args, envp, data);
	ft_closing_all(&fds);
	if (args && args[0] && cmd_path)
		execve(cmd_path, args, envp);
	if (!cmd_path)
	{
		if (args && args[0])
		{
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		secure_exit(data, CMD_NOT_FOUND);
	}
	if (cmd_path)
		free(cmd_path);
	secure_exit(data, FAIL_STATUS);
}

static void	apply_redirs_dup(t_data *data, t_ast **node)
{
	int		r;

	r = 0;
	while (r < (*node)->cmd->redir_count)
	{
		heredoc_dup(data, r);
		file_dup(data, node, r);
		r++;
	}
}

static void	heredoc_dup(t_data *data, int r)
{
	t_fds	*fds;
	t_ast	*node;

	node = data->tree;
	fds = data->fds;
	if (node->cmd->redirs[r].label == REDIR_HEREDOCK)
	{
		secure_close(&fds->heredoc_fds[fds->pos.doc_id][1]);
		if (fds->heredoc_fds[fds->pos.doc_id][0] != -1
		&& dup2(fds->heredoc_fds[fds->pos.doc_id][0], STDIN_FILENO) == -1)
			secure_exit(data, 1);
		fds->pos.doc_id++;
	}
}

static void	file_dup(t_data *data, t_ast **node, int r)
{
	t_fds	*fds;

	fds = data->fds;
	if ((*node)->cmd->redirs[r].label == REDIR_IN)
	{
		if (dup2(fds->fd_files[fds->pos.file_id], STDIN_FILENO
			) == -1)
		{
			fds->pos.file_id++;
			secure_exit(data, 1);
		}
		fds->pos.file_id++;
	}
	else if ((*node)->cmd->redirs[r].label == REDIR_OUT
		|| (*node)->cmd->redirs[r].label == REDIR_APPEND)
	{
		if (dup2(fds->fd_files[fds->pos.file_id], STDOUT_FILENO
			) == -1)
		{
			fds->pos.file_id++;
			secure_exit(data, 1);
		}
		fds->pos.file_id++;
	}
}
