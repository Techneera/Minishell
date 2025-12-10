/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_and_purge.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:07:16 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/07 21:44:00 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "expansion.h"
#include "execution.h"

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

void	ft_restore_markers(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		if (args[i][0] == '\1')
			args[i][0] = '\0';
		i++;
	}
}

int	ft_handle_wildcard(t_data *data, int i, char *str)
{
	t_list	*matches;

	if (!ft_strchr(str, '*'))
		return (-1);
	matches = get_wildcard_matches(str);
	if (!matches)
		return (-1);
	data->tree->cmd->args = insert_wildcard_args(data->tree->cmd->args, \
matches, i);
	i += ft_lstsize(matches);
	ft_lstclear(&matches, free);
	return (i);
}

int	ft_process_arg(t_data *data, int i)
{
	int		quoted;
	char	*tmp;
	int		new_idx;

	quoted = has_quotes(data->tree->cmd->args[i]);
	tmp = expand_word(data->tree->cmd->args[i], \
data->envp, ft_exit_status(0, 0, 0));
	data->tree->cmd->args[i] = tmp;
	new_idx = ft_handle_wildcard(data, i, tmp);
	if (new_idx != -1)
		return (new_idx);
	unmask_wildcards(tmp);
	if (tmp[0] == '\0' && quoted)
	{
		free(data->tree->cmd->args[i]);
		data->tree->cmd->args[i] = ft_strdup("\1");
	}
	return (i + 1);
}

void	expand_args(t_data *data)
{
	int	i;

	i = 0;
	while (data->tree->cmd->args[i])
	{
		i = ft_process_arg(data, i);
	}
	data->tree->cmd->args = ft_realloc_empty(data->tree->cmd->args);
	ft_restore_markers(data->tree->cmd->args);
}
