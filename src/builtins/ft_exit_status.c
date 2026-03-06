/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_status.c                                   :+:      :+:    :+:   */
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
void	ft_handle_overflow(t_data *data, int i)
{
	ft_exit_status(ft_error(2, data->tree->cmd->args[i], \
"numeric argument required"), 1, 0);
	secure_exit(data, 2);
}

/**
 * \brief Built-in `exit`: exit the shell with an optional numeric status.
 *
 * Prints "exit" to stderr, validates the argument count and numeric range,
 * then calls secure_exit() with the status truncated to 0–255.
 * \param data The shell state.
 * \return 1 on "too many arguments"; does not return on success.
 */
int	ft_exit(t_data *data)
{
	char		**args;
	long long	exit_val;
	int			i;

	ft_exit_print();
	i = 1;
	if (!data->tree)
		secure_exit(data, 0);
	args = data->tree->cmd->args;
	if (!args[1])
		secure_exit(data, ft_exit_status(0, 0, 0));
	if (!ft_strncmp(args[1], "--", 3))
	{
		i = 2;
		if (!args[i])
			secure_exit(data, ft_exit_status(0, 0, 0));
	}
	if (!ft_safe_atoll(args[i], &exit_val))
		ft_handle_overflow(data, i);
	if (args[i + 1])
		return (ft_error(1, NULL, "too many arguments"));
	secure_exit(data, (int)(exit_val % 256));
	return (0);
}

/**
 * \brief Get, set, or act on the shell's last exit status (static variable).
 *
 * Serves three purposes depending on the flags:
 * - Read-only (`write_` = 0, `exit_` = 0): return current status.
 * - Write (`write_` != 0): update the stored status.
 * - Exit (`exit_` != 0): call exit() with the current status.
 * \param state  New status value (applied only when \c write_ is non-zero).
 * \param write_ Non-zero to store \c state.
 * \param exit_  Non-zero to call \c exit() immediately.
 * \return The current (or newly set) exit status.
 */
int	ft_exit_status(int state, int write_, int exit_)
{
	static int	current_status;

	if (write_ != 0)
		current_status = state;
	if (exit_ != 0)
		exit(current_status);
	return (current_status);
}
