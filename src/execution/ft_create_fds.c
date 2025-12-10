/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_fds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static void	create_files(t_data *data);

void	ft_create_fds(t_data *data)
{
	t_fds	*fds;
	t_ast	*ast_root;

	if (!data->tree)
		return ;
	ast_root = data->tree;
	fds = ft_calloc(1, sizeof(t_fds));
	if (!fds)
		secure_exit(data, FAIL_STATUS);
	get_sizes(ast_root, &fds, 0);
	if (fds->get.n_cmds > 0)
	{
		fds->c_pids = ft_calloc(fds->get.n_cmds, sizeof(int));
		if (!fds->c_pids)
			secure_exit(data, FAIL_STATUS);
	}
	data->fds = fds;
	if (init_heredoc(data) == -1)
		return ;
	create_files(data);
}

static void	create_files(t_data *data)
{
	t_fds	*fds;
	int		i;

	i = 0;
	fds = data->fds;
	if (fds->get.n_files > 0)
	{
		fds->fd_files = ft_calloc(fds->get.n_files, sizeof(int));
		if (!fds->fd_files)
			secure_exit(data, FAIL_STATUS);
		while (++i < fds->get.n_files)
			fds->fd_files[i] = -1;
	}
	fill_fd_file(data, data->tree, 0);
}
