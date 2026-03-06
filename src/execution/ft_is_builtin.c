/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

/**
 * \brief Identify whether the command is a built-in and execute it in-process.
 *
 * Compares \c arg against the seven supported built-in names.
 * If a match is found the built-in is called and the exit status is updated.
 * \param data The shell state.
 * \param arg  The command name (args[0]).
 * \return 1 if a built-in was dispatched; 0 if the command is external.
 */
int	ft_is_builtin(t_data *data, char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "echo\0", 5) == 0)
		return (ft_exit_status(ft_echo(data), 1, 0), 1);
	if (ft_strncmp(arg, "cd\0", 3) == 0)
		return (ft_exit_status(ft_cd(data->tree, data->envp, data), 1, 0), 1);
	if (ft_strncmp(arg, "pwd\0", 4) == 0)
		return (ft_exit_status(ft_pwd(data), 1, 0), 1);
	if (ft_strncmp(arg, "export\0", 7) == 0)
		return (ft_exit_status(ft_export(
					data->env_list, data->tree->cmd->args, data), 1, 0), 1);
	if (ft_strncmp(arg, "unset\0", 6) == 0)
		return (ft_exit_status(ft_unset(data), 1, 0), 1);
	if (ft_strncmp(arg, "env\0", 4) == 0)
		return (ft_exit_status(ft_env(data), 1, 0), 1);
	if (ft_strncmp(arg, "exit\0", 5) == 0)
		return (ft_exit_status(ft_exit(data), 1, 0), 1);
	return (0);
}
