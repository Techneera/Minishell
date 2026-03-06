/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/07 16:07:59 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_H
# define LEXER_H

# include "libshell.h"

/**
 * \brief Initialise a lexer for the given input line.
 * \param line The raw command-line string (not owned by the lexer).
 * \return Heap-allocated \c t_lexer, or NULL on allocation failure.
 */
t_lexer	*ft_state_lexer(char *line);

/**
 * \brief Fetch the next token from the lexer.
 * \param l The lexer state.
 * \return Heap-allocated \c t_token, or NULL on allocation failure.
 *         Returns a TOKEN_EOF token when input is exhausted.
 */
t_token	*get_next_token(t_lexer *l);

/**
 * \brief Dispatch operator tokenisation based on the current character.
 * \param l The lexer state positioned at a meta-character.
 * \return Heap-allocated token, or TOKEN_ERROR if unsupported.
 */
t_token	*ft_handle_operator(t_lexer *l);

/**
 * \brief Disambiguate `|` (pipe) from `||` (logical OR).
 * \param l The lexer state positioned at `|`.
 * \return TOKEN_PIPE or TOKEN_OR token.
 */
t_token	*ft_which_or(t_lexer *l);

/**
 * \brief Disambiguate `&&` from a bare `&` (which is an error).
 * \param l The lexer state positioned at `&`.
 * \return TOKEN_AND, or TOKEN_ERROR for a bare `&`.
 */
t_token	*ft_which_and(t_lexer *l);

/**
 * \brief Disambiguate `<` / `>` / `<<` / `>>` redirection tokens.
 * \param l The lexer state positioned at `<` or `>`.
 * \return The appropriate redirection token.
 */
t_token	*ft_which_redir(t_lexer *l);

/**
 * \brief Tokenise a word (possibly quoted) from the input.
 * \param l The lexer state positioned at the start of a word.
 * \return TOKEN_WORD token, or TOKEN_ERROR if quotes are unclosed.
 */
t_token	*ft_handle_word(t_lexer *l);

/**
 * \brief Allocate and initialise a new token.
 * \param tok_label The semantic label for the token.
 * \param str       Heap-allocated string value (ownership is transferred).
 * \return Heap-allocated \c t_token, or NULL on allocation failure.
 */
t_token	*create_token(t_token_label tok_label, char *str);

/**
 * \brief Extract and unescape the heredoc delimiter from the input.
 * \param l The lexer state positioned after `<<`.
 * \return Heap-allocated unquoted delimiter string, or NULL on failure.
 */
char	*ft_get_unquoted_str(t_lexer *l);

/**
 * \brief Strip quoting characters from a raw heredoc delimiter.
 * \param raw_delim   Pointer to the raw delimiter string (will be freed).
 * \param final_delim Output pointer for the cleaned delimiter.
 * \param len         Length of the raw delimiter.
 * \return The cleaned delimiter string, or NULL on allocation failure.
 */
char	*ft_heredoc_quotes(char **raw_delim, char **final_dleim, int len);

/**
 * \brief Advance the lexer past whitespace to find the heredoc delimiter.
 * \param start         Output: start position of the delimiter.
 * \param in_quote_state Output: non-zero if the delimiter is quoted.
 * \param l             The lexer state.
 */
void	initialize_and_find_heredoc_delim(int *start, \
int *in_quote_state, t_lexer *l);

/**
 * \brief Append a token to the end of a token list.
 * \param tok_label Pointer to the head of the token list.
 * \param new       The token to append.
 */
void	add_token_back(t_token **tok_label, t_token *new);

/**
 * \brief Tokenise the entire lexer input into a token list.
 * \param lex  Pointer to the lexer (updated in place).
 * \param head Output pointer for the head of the resulting token list.
 */
void	ft_tokens_constructor(t_lexer **lex, t_token **head);

/**
 * \brief Free every node in a token linked list.
 * \param tok_label Pointer to the head of the list.
 * \param del       Destructor called on each \c t_token node.
 */
void	free_token_lst(t_token **tok_label, void (*del)(t_token *));

/**
 * \brief Free a single token and its string.
 * \param t The token to free (may be NULL).
 */
void	free_token(t_token *t);

/**
 * \brief Free the lexer struct (does not free the input string).
 * \param l The lexer to free (may be NULL).
 */
void	free_lexer(t_lexer *l);

/**
 * \brief Test whether a character is a shell meta-character.
 * \param c The character to test.
 * \return Non-zero if \c c is `|`, `&`, `<`, `>`, `(`, or `)`.
 */
int		ft_ismeta(int c);

/**
 * \brief Handle the second character after `>` (disambiguate `>` vs `>>`).
 * \param l The lexer state positioned at the second `>`.
 * \return TOKEN_REDIR_OUT or TOKEN_REDIR_APPEND token.
 */
t_token	*ft_handle_output_redir_aux(t_lexer *l);

/**
 * \brief Handle the heredoc `<<` token, consuming and storing the delimiter.
 * \param l The lexer state positioned after the first `<`.
 * \return TOKEN_REDIR_HEREDOC token whose \c str is the unquoted delimiter.
 */
t_token	*ft_handle_heredoc_aux(t_lexer *l);

#endif
