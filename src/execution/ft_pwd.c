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

int	ft_pwd(t_data *data)
{
	char	*path;
	int		fd;

	fd = ft_target_fd(data);
	if (fd == -1)
		return (1);
	path = ft_getenv(data->envp, "PWD");
	if (path)
	{
		ft_putstr_fd(ft_getenv(data->envp, "PWD"), fd);
		ft_putstr_fd("\n", fd);
		return (0);
	}
	return (FAIL_STATUS);
}
