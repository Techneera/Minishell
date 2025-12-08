/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child_sshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds);

static
void	ft_child_sshell_aux(t_data *data, int *i, int *child_status)
{
	*i = -1;
	*child_status = 0;
	data->fds->get.n_cmds = ft_n_cmds_sshell(data->tree->body, &data->fds);
}

void	ft_child_sshell(t_data *data, char **envp)
{
	int		child_status;
	int		i;

	ft_child_sshell_aux(data, &i, &child_status);
	apply_redirs_subshell(data);
	if (data->fds->c_pids)
		free(data->fds->c_pids);
	data->fds->c_pids = ft_calloc(data->fds->get.n_cmds, sizeof(int));
	data->fds->pos.fork_id = 0;
	if (!data->fds->c_pids)
		secure_exit(data, 0);
	data->tree = data->tree->body;
	ft_exec_tree(data, envp);
	ft_closing_all(&data->fds);
	while (data->fds && ++i < data->fds->get.n_cmds && data->fds->c_pids)
	{
		if (waitpid(data->fds->c_pids[i], &child_status, 0) > 0)
		{
			if (WIFEXITED(child_status))
				ft_exit_status(WEXITSTATUS(child_status), 1, 0);
			else if (WIFSIGNALED(child_status))
				ft_exit_status(WTERMSIG(child_status) + 128, 1, 0);
		}
	}
	secure_exit(data, ft_exit_status(0, 0, 0));
}

static int	ft_n_cmds_sshell(t_ast *ast_root, t_fds **fds)
{
	int	i;

	i = 0;
	if (!ast_root)
		return (i);
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
		return (i);
	i += ft_n_cmds_sshell(ast_root->left, fds);
	i += ft_n_cmds_sshell(ast_root->right, fds);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
		i++;
	return (i);
}
