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
#include "libshell.h"
#include "execution.h"

static
int	is_valid_n_option(char *arg)
{
	int		i;

	i = 1;
	if (!arg)
		return (0);
	if (arg[0] != '-')
		return (0);
	if (arg[1] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static
void	ft_validate_args(int *i, int *print_newline, char **args)
{
	while (args[++(*i)])
	{
		if (is_valid_n_option(args[*i]) == 1)
			*print_newline = 0;
		else
			break ;
	}
}

int	ft_target_fd(t_data *data)
{
	int	i;
	int	j;
	int	fd;

	i = 0;
	j = 0;
	fd = 1;
	while (j < data->tree->cmd->redir_count)
	{
		if (data->tree->cmd->redirs[j].label == REDIR_OUT || \
data->tree->cmd->redirs[j].label == REDIR_APPEND)
		{
			if (data->fds->fd_files[data->fds->pos.file_id + i] == -1)
				return (-1);
			fd = data->fds->fd_files[data->fds->pos.file_id + i];
		}
		if (data->fds->fd_files[data->fds->pos.file_id + i] == -1)
			return (-1);
		if (data->tree->cmd->redirs[j].label != REDIR_HEREDOCK)
			i++;
		j++;
	}
	return (fd);
}

int	ft_echo(t_data *data)
{
	char	**args;
	int		i;
	int		print_newline;
	int		fd;

	fd = ft_target_fd(data);
	if (fd == -1)
		return (1);
	args = data->tree->cmd->args;
	print_newline = 1;
	i = 0;
	ft_validate_args(&i, &print_newline, args);
	while (args[i])
	{
		ft_putstr_fd(args[i], fd);
		if (args[i + 1] != NULL)
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (print_newline == 1)
		ft_putstr_fd("\n", fd);
	return (0);
}
