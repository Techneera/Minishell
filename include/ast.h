/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef AST_H
# define AST_H

# include "libshell.h"

/**
 * \brief Allocate a NODE_CMD AST node wrapping the given command.
 * \param cmd The command to embed in the node (ownership transferred).
 * \return Heap-allocated \c t_ast node, or NULL on failure.
 */
t_ast			*ft_ast_node_command(t_cmd *cmd);

/**
 * \brief Allocate a generic AST node of the specified type.
 * \param type The node type (e.g. NODE_PIPE, NODE_AND, NODE_OR, NODE_SUBSHELL).
 * \return Heap-allocated \c t_ast node with all pointer fields NULL, or NULL.
 */
t_ast			*ft_ast_generic_node(t_node_type type);

/**
 * \brief Initialise a parser with the first two look-ahead tokens.
 * \param l The lexer to draw tokens from.
 * \return Heap-allocated \c t_parser, or NULL on allocation failure.
 */
t_parser		*ft_init_parser(t_lexer *l);

/**
 * \brief Allocate a t_cmd holding the provided argument vector and redirections.
 * \param av     NULL-terminated argument array (ownership transferred).
 * \param redirs Array of \c count redirection descriptors (ownership transferred).
 * \param count  Number of entries in \c redirs.
 * \return Heap-allocated \c t_cmd, or NULL on allocation failure.
 */
t_cmd			*ft_create_command(char **av, t_redir *redirs, int count);

/**
 * \brief Advance the parser to the next token.
 * \param parser The parser state to advance.
 */
void			ft_parser_iter(t_parser *parser);

/**
 * \brief Zero-initialise a simple-command accumulator context.
 * \param ctx The context to initialise.
 */
void			ft_init_simp_ctx(t_simp_ctx *ctx);

/**
 * \brief Release all resources held by a simple-command accumulator.
 * \param ctx The context to free.
 * \return Always NULL (for use in return expressions).
 */
void			*ft_free_simp_ctx(t_simp_ctx *ctx);

/**
 * \brief Process a TOKEN_WORD by appending it to the argument list.
 * \param p   The parser state.
 * \param ctx The accumulator context being built.
 * \return 1 on success, 0 on allocation failure.
 */
int				ft_handle_word_token(t_parser *p, t_simp_ctx *ctx);

/**
 * \brief Process a redirection token by appending it to the redir list.
 * \param p   The parser state.
 * \param ctx The accumulator context being built.
 * \return 1 on success, 0 on failure.
 */
int				ft_handle_redir_token(t_parser *p, t_simp_ctx *ctx);

/**
 * \brief Finalise a simple command: convert lists to arrays and build the AST node.
 * \param ctx The accumulator context (ownership of its lists is consumed).
 * \return Heap-allocated NODE_CMD AST node, or NULL on failure.
 */
t_ast			*ft_finalize_simp_cmd(t_simp_ctx *ctx);

/**
 * \brief Test whether the parser's current token is a redirection.
 * \param parser The parser state.
 * \return Non-zero if the current token is `<`, `>`, `>>`, or `<<`.
 */
int				ft_isredir(t_parser *parser);

/**
 * \brief Process one redirection token in a post-subshell redir loop.
 * \param p   The parser state.
 * \param lst Pointer to the accumulating redir list.
 * \param cmd The command node whose redir_count is incremented.
 * \return 1 on success, 0 on failure.
 */
int				ft_proc_redir_loop(t_parser *p, t_list **lst, t_cmd *cmd);

/**
 * \brief Extract filename / delimiter from a redirection token.
 * \param p     The parser state.
 * \param label The redirection type already identified.
 * \return Heap-allocated \c t_redir, or NULL on failure.
 */
t_redir			*ft_extract_redir_info(t_parser *p, t_label_redir label);

/**
 * \brief Map a token label to the corresponding redirection label.
 * \param label A redirection token label.
 * \return The matching \c t_label_redir value, or REDIR_NONE if not a redir.
 */
t_label_redir	ft_label_map(t_token_label label);

/**
 * \brief Entry point: tokenise \c l and build the full AST.
 * \param l The lexer for the current input line.
 * \return Root of the AST, or NULL on parse / allocation error.
 */
t_ast			*ft_parser(t_lexer *l);

/**
 * \brief Parse a logical `&&` / `||` expression (lowest precedence).
 * \param parser The parser state.
 * \return AST node for the and/or expression, or NULL on error.
 */
t_ast			*ft_parse_and_or(t_parser *parser);

/**
 * \brief Parse a pipeline (one or more commands separated by `|`).
 * \param parser The parser state.
 * \return AST node for the pipeline, or NULL on error.
 */
t_ast			*ft_parse_pipeline(t_parser *parser);

/**
 * \brief Parse a single pipeline stage, optionally preceded by redirections.
 * \param parser The parser state.
 * \return AST node (NODE_CMD or NODE_SUBSHELL), or NULL on error.
 */
t_ast			*ft_parse_grain_with_redirs(t_parser *parser);

/**
 * \brief Parse a command node (currently an alias for ft_parse_simple_command).
 * \param parser The parser state.
 * \return AST node, or NULL on error.
 */
t_ast			*ft_parse_node_command(t_parser *parser);

/**
 * \brief Parse a `( and_or )` subshell expression.
 * \param parser The parser state positioned at `(`.
 * \return NODE_SUBSHELL AST node, or NULL on syntax / allocation error.
 */
t_ast			*ft_parse_subshell(t_parser *parser);

/**
 * \brief Parse a simple command: words and redirections until a meta-token.
 * \param parser The parser state.
 * \return NODE_CMD AST node, or NULL on error.
 */
t_ast			*ft_parse_simple_command(t_parser *parser);

/**
 * \brief Consume a TOKEN_WORD and push it onto the argument list.
 * \param parser The parser state.
 * \param head   Pointer to the accumulating argument linked list.
 */
void			ft_parse_args(t_parser *parser, t_list **head);

/**
 * \brief Convert a linked list of argument strings to a NULL-terminated array.
 * \param head Pointer to the head of the list (consumed and NULLed).
 * \param size Number of nodes in the list.
 * \return Heap-allocated \c char** array, or NULL on failure.
 */
char			**ft_lst_to_args(t_list **head, int size);

/**
 * \brief Parse zero or more trailing redirections into an existing command.
 * \param parser      The parser state.
 * \param cmd_to_fill The command whose \c redirs array will be populated.
 * \return true on success, false on syntax / allocation error.
 */
int				ft_handle_redirects(t_parser *parser, t_cmd *cmd_to_fill);

/**
 * \brief Allocate a t_redir with the given label and filename.
 * \param label The redirection type.
 * \param str   Heap-allocated filename string (ownership transferred).
 * \return Heap-allocated \c t_redir, or NULL on failure (frees \c str).
 */
t_redir			*ft_create_redir(t_label_redir label, char *str);

/**
 * \brief Parse a single redirection token into a t_redir.
 * \param parser The parser state positioned at the redirection token.
 * \return Heap-allocated \c t_redir, or NULL on error.
 */
t_redir			*ft_parse_single_redir(t_parser *parser);

/**
 * \brief Free a t_redir struct only (does NOT free the filename string).
 * \param content Pointer to the \c t_redir to free (cast from void*).
 */
void			ft_free_redir_struct_only(void *content);

/**
 * \brief Free a t_redir struct and its filename string.
 * \param content Pointer to the \c t_redir to free (cast from void*).
 */
void			ft_free_redir_content(void *content);

/**
 * \brief Copy a linked list of t_redir nodes into a flat array.
 * \param head  The source linked list.
 * \param array The destination array (must be pre-allocated).
 */
void			ft_copy_lst_to_array(t_list *head, t_redir *array);

/**
 * \brief Free a NULL-terminated array of heap-allocated strings.
 * \param arr The array to free (may be NULL).
 */
void			ft_free_array(char **arr);

/**
 * \brief Recursively free a t_cmd and all its members.
 * \param cmd The command to free (may be NULL).
 */
void			ft_free_cmd(t_cmd *cmd);

/**
 * \brief Recursively free an entire AST.
 * \param root The root node to free (may be NULL).
 */
void			ft_free_ast(t_ast *root);

#endif
