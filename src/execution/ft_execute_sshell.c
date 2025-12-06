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

static void	update_fds_subshell(t_ast *ast_root, t_fds **fds);

void	ft_execute_sshell(t_data *data, char **envp)
{
	pid_t	pid;

	if (!data->tree)
		return ;
	if (!init_pid(&pid, &data->fds))
		secure_exit(data, FAIL_STATUS);
	if (pid == 0)
		ft_child_sshell(data, envp);
	update_fds_subshell(data->tree, &data->fds);
}

static void	update_fds_subshell(t_ast *ast_root, t_fds **fds)
{
	int	i;

	if (!ast_root)
		return ;
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
	{
		(*fds)->pos.doc_id += docs_bonus(ast_root, fds);
		return ;
	}
	i = 0;
	update_fds_subshell(ast_root->body, fds);
	update_fds_subshell(ast_root->left, fds);
	update_fds_subshell(ast_root->right, fds);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label != REDIR_HEREDOCK)
				(*fds)->pos.file_id++;
			if (ast_root->cmd->redirs[i].label == REDIR_HEREDOCK)
				(*fds)->pos.doc_id++;
			i++;
		}
	}
}
