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

char	**get_paths(char **env)
{
	char	**paths;
	char	**paths2;
	int		i;

	i = -1;
	while (env && env[++i])
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
	if (env && env[i])
	{
		paths = ft_split(env[i], '=');
		paths2 = ft_split(paths[1], ':');
		free_all((void **) paths, ft_arraylen((void **) paths));
		return (paths2);
	}
	return (NULL);
}

static void	increment_sizes(t_ast *ast_root, t_fds **fds, int inside_sshell)
{
	int	i;

	i = 0;
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		if (!inside_sshell)
			(*fds)->get.n_cmds++;
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label != REDIR_HEREDOCK)
				(*fds)->get.n_files++;
			if (ast_root->cmd->redirs[i].label == REDIR_HEREDOCK)
				(*fds)->get.n_docs++;
			i++;
		}
	}
}

void	get_sizes(t_ast *ast_root, t_fds **fds, int inside_sshell)
{
	if (!ast_root)
		return ;
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
	{
		(*fds)->get.n_docs += docs_bonus(ast_root, fds);
		return ;
	}
	if (ast_root->type == NODE_SUBSHELL)
		get_sizes(ast_root->body, fds, 1);
	get_sizes(ast_root->left, fds, inside_sshell);
	get_sizes(ast_root->right, fds, inside_sshell);
	increment_sizes(ast_root, fds, inside_sshell);
}
