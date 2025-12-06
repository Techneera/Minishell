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

static int	malloc_heredoc(t_fds **fds);
static int	fill_heredoc(t_data *data, t_ast *node, int i);

int	init_heredoc(t_data *data)
{
	t_fds	*fds;
	t_ast	*node;

	fds = data->fds;
	node = data->tree;
	fds->heredoc_fds = NULL;
	if (fds->get.n_docs > 0)
	{
		fds->heredoc_fds = ft_calloc(fds->get.n_docs, sizeof(int *));
		if (!fds->heredoc_fds)
		{
			free_data(data);
			return (-1);
		}
		if (!malloc_heredoc(&fds))
		{
			free_data(data);
			return (-1);
		}
		return (fill_heredoc(data, node, 0));
	}
	return (0);
}

static int	malloc_heredoc(t_fds **fds)
{
	int	i;

	i = 0;
	while (i < (*fds)->get.n_docs)
	{
		(*fds)->heredoc_fds[i] = malloc(2 * sizeof(int));
		if (!(*fds)->heredoc_fds[i])
			return (free_all((void **)(*fds)->heredoc_fds, \
(*fds)->get.n_docs), 0);
		if (pipe((*fds)->heredoc_fds[i]) == -1)
			return (free_all((void **)(*fds)->heredoc_fds, \
(*fds)->get.n_docs), 0);
		i++;
	}
	return (1);
}

static int	fill_heredoc(t_data *data, t_ast *node, int i)
{
	int		y;
	t_fds	*fds;

	fds = data->fds;
	y = 0;
	if (!node)
		return (i);
	i = fill_heredoc(data, node->body, i);
	if (i != -1)
		i = fill_heredoc(data, node->left, i);
	if (i != -1)
		i = fill_heredoc(data, node->right, i);
	if (i != -1 && (!node->cmd || !node->cmd->redirs))
		return (i);
	while (i != -1 && y < node->cmd->redir_count)
	{
		if (node->cmd->redirs[y].label == REDIR_HEREDOCK)
		{
			if (!here_doc(node->cmd->redirs[y].file_name, &fds->heredoc_fds[i]))
				return (free_data(data), -1);
			i++;
		}
		y++;
	}
	return (i);
}
