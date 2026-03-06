/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

/**
 * \brief SIGINT handler at the interactive prompt (Ctrl-C).
 *
 * Prints a newline, resets the readline line buffer, re-displays
 * the prompt, and sets the exit status to 130.
 * \param sig The signal number (unused).
 */
void	handle_sigstop(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	ft_exit_status(130, 1, 0);
}

/**
 * \brief SIGINT handler while waiting for a child to finish.
 *
 * Prints a newline and records exit status 130.
 * \param sig The signal number (unused).
 */
void	handle_sigint_wait(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	ft_exit_status(130, 1, 0);
}

/**
 * \brief SIGINT handler during heredoc input (Ctrl-C).
 *
 * Closes stdin so the heredoc read loop exits, sets exit status 130,
 * and flags the heredoc as interrupted via heredoc_status().
 * \param sig The signal number (unused).
 */
void	handle_sigstop_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	ft_exit_status(130, 1, 0);
	heredoc_status(1, 1);
}

/**
 * \brief Get or set the heredoc-interrupted flag (static variable).
 * \param state  New flag value (written only when \c write_ is non-zero).
 * \param write_ Non-zero to overwrite the stored value; 0 to read only.
 * \return The current value of the flag.
 */
int	heredoc_status(int state, int write_)
{
	static int	to_clear;

	if (!to_clear)
		to_clear = 0;
	if (write_ != 0)
		to_clear = state;
	return (to_clear);
}
