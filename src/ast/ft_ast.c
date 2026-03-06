/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
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
 * \brief Allocate a NODE_CMD AST node containing the given command.
 * \param cmd The command to embed (ownership transferred to the node).
 * \return Heap-allocated NODE_CMD node, or NULL on failure.
 */
t_ast	*ft_ast_node_command(t_cmd *cmd)
{
	t_ast	*new_node;

	if (!cmd)
		return (NULL);
	new_node = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = NODE_CMD;
	new_node->cmd = cmd;
	new_node->body = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

/**
 * \brief Allocate a generic AST node of the given type with all pointers NULL.
 * \param type The node type to assign.
 * \return Heap-allocated node, or NULL on failure.
 */
t_ast	*ft_ast_generic_node(t_node_type type)
{
	t_ast	*new_node;

	new_node = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->cmd = NULL;
	new_node->body = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

/**
 * \brief Create and initialise a parser backed by the given lexer.
 *
 * Pre-loads \c current_token and \c peek so the parser always has
 * one-token look-ahead without any special-case logic.
 * \param l The lexer to draw tokens from.
 * \return Heap-allocated \c t_parser, or NULL on allocation failure.
 */
t_parser	*ft_init_parser(t_lexer *l)
{
	t_parser	*parser;

	parser = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->lex = l;
	parser->current_token = get_next_token(l);
	if (!parser->current_token)
	{
		free(parser);
		return (NULL);
	}
	parser->peek = get_next_token(l);
	if (!parser->peek)
	{
		free(parser->current_token);
		free(parser);
		return (NULL);
	}
	return (parser);
}

/**
 * \brief Advance the parser by one token.
 *
 * The old \c current_token is freed, \c peek becomes the new current,
 * and a new peek is fetched from the lexer.  Does nothing at EOF.
 * \param parser The parser to advance.
 */
void	ft_parser_iter(t_parser *parser)
{
	if (parser->current_token->tok_label == TOKEN_EOF)
		return ;
	free_token(parser->current_token);
	parser->current_token = parser->peek;
	parser->peek = get_next_token(parser->lex);
}

/**
 * \brief Allocate a t_cmd and fill it with the provided argument vector and redirections.
 * \param av     NULL-terminated argument array (may be NULL for empty commands).
 * \param redirs Array of \c count redirection descriptors (may be NULL).
 * \param count  Number of elements in \c redirs.
 * \return Heap-allocated \c t_cmd, or NULL on failure.
 */
t_cmd	*ft_create_command(char	**av, t_redir *redirs, int count)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = av;
	new_cmd->redir_count = count;
	new_cmd->redirs = redirs;
	return (new_cmd);
}
