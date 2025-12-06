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

int	ft_env(t_data *data)
{
	t_list	*list;
	t_env	*env;
	int		fd;

	fd = ft_target_fd(data);
	if (fd == -1)
		return (1);
	list = data->env_list;
	if (!list)
		return (FAIL_STATUS);
	while (list)
	{
		env = (t_env *) list->content;
		if (env->has_arg)
		{
			ft_putstr_fd(env->variable, fd);
			ft_putstr_fd("\n", fd);
		}
		list = list->next;
	}
	return (0);
}
