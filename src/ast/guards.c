/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guards.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "ast.h"
#include "lexer.h"

/**
 * \brief Free a NULL-terminated array of heap-allocated strings.
 * \param arr The array to free (safe to call with NULL).
 */
void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = NULL;
}

/**
 * \brief Free a t_cmd and all its member arrays.
 *
 * Frees the argument array, each redirection's filename, the
 * redirection array itself, and the \c t_cmd struct.
 * \param cmd The command to free (safe to call with NULL).
 */
void	ft_free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	ft_free_array(cmd->args);
	if (cmd->redirs)
	{
		i = 0;
		while (i < cmd->redir_count)
		{
			free(cmd->redirs[i].file_name);
			i++;
		}
		free(cmd->redirs);
	}
	free(cmd);
	cmd = NULL;
}

/**
 * \brief Recursively free an entire AST and all its commands / strings.
 *
 * For AND/OR/PIPE nodes the left and right children are freed first.
 * For SUBSHELL nodes the body subtree is freed.
 * The \c cmd is always freed last via \c ft_free_cmd().
 * \param root The root of the AST to free (safe to call with NULL).
 */
void	ft_free_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->type == NODE_AND || root->type == NODE_PIPE \
|| root->type == NODE_OR)
	{
		ft_free_ast(root->left);
		ft_free_ast(root->right);
	}
	else if (root->type == NODE_SUBSHELL)
		ft_free_ast(root->body);
	ft_free_cmd(root->cmd);
	free(root);
	root = NULL;
}

/**
 * \brief Free a t_redir struct without freeing its filename string.
 *
 * Used as a list destructor when the filename ownership has already
 * been transferred to the final array.
 * \param content The \c t_redir pointer cast to void*.
 */
void	ft_free_redir_struct_only(void *content)
{
	t_redir	*redir;

	redir = (t_redir *)content;
	if (!redir)
		return ;
	free(redir);
	redir = NULL;
}
