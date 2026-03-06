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

/**
 * \brief Test whether a string contains any single or double quote character.
 * \param str The string to inspect (may be NULL).
 * \return Non-zero if at least one quote is present; 0 otherwise.
 */
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

/**
 * \brief Restore the empty-argument marker byte `\001` back to `\0`.
 *
 * Arguments that expanded to an empty quoted string (e.g. `""`) are
 * temporarily marked with `\001` as their first byte so they survive
 * array compaction.  This pass restores them to proper empty strings.
 * \param args NULL-terminated argument array modified in place (may be NULL).
 */
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

/**
 * \brief Expand a single argument that contains `*` into matching filenames.
 *
 * If no `*` is present or no filesystem matches are found, returns -1
 * so the caller keeps the original argument unchanged.
 * \param data The shell state (args array is reallocated on match).
 * \param i    The index of the argument to replace.
 * \param str  The argument string (must already be variable-expanded).
 * \return The new index after the inserted matches, or -1 on no match.
 */
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

/**
 * \brief Expand one argument: variable expansion → wildcard glob.
 *
 * Calls expand_word() to perform variable and quote expansion, then
 * optionally replaces the result with glob matches.  Empty quoted
 * strings are marked with `\001` to prevent later removal.
 * \param data The shell state.
 * \param i    Index of the argument to process.
 * \return The next index to process (may skip ahead after glob expansion).
 */
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

/**
 * \brief Expand all arguments of the current command node in place.
 *
 * Iterates over \c data->tree->cmd->args, calling ft_process_arg() on
 * each one.  After all arguments are processed, compacts the array
 * (ft_realloc_empty) and restores empty-argument markers.
 * \param data The shell state whose current command arguments are expanded.
 */
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
