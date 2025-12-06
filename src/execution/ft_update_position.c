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

void	update_doc_in_bonus(t_ast *node, t_data *data);

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

void	update_from_position(t_ast *node, t_data *data)
{
	t_ast	*holder;

	holder = data->tree;
	if (!node)
		return ;
	update_from_position(node->left, data);
	update_from_position(node->right, data);
	if (node->type == NODE_AND || node->type == NODE_OR \
|| node->type == NODE_SUBSHELL)
	{
		update_doc_in_bonus(node, data);
		return ;
	}
	if (node->type == NODE_CMD)
	{
		data->tree = node;
		update_positions(data);
		data->tree = holder;
	}
}

void	update_doc_in_bonus(t_ast *node, t_data *data)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	update_doc_in_bonus(node->body, data);
	update_doc_in_bonus(node->left, data);
	update_doc_in_bonus(node->right, data);
	if (node->type == NODE_CMD || node->type == NODE_SUBSHELL)
	{
		while (i < node->cmd->redir_count)
		{
			if (node->cmd->redirs[i].label == REDIR_HEREDOCK)
				data->fds->get.n_docs++;
			i++;
		}
	}
}
