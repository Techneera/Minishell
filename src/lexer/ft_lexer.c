/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "lexer.h"

/**
 * \brief Test whether a character is a shell meta-character.
 * \param c The character to test.
 * \return Non-zero if \c c is one of `|`, `&`, `<`, `>`, `(`, `)`.
 */
int	ft_ismeta(int c)
{
	return (c == '|' || c == '&' || c == '<' || \
c == '>' || c == '(' || c == ')');
}

/**
 * \brief Allocate and initialise a lexer for the given input string.
 *
 * The lexer does not take ownership of \c line; the caller is
 * responsible for keeping it alive until \c free_lexer() is called.
 * \param line The raw input string to tokenise.
 * \return Heap-allocated \c t_lexer, or NULL on allocation failure.
 */
t_lexer	*ft_state_lexer(char *line)
{
	t_lexer	*ptr;

	ptr = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!ptr)
		return (NULL);
	ptr->input = line;
	ptr->pos = 0;
	ptr->len = ft_strlen(line);
	return (ptr);
}

/**
 * \brief Fetch the next token from the lexer.
 *
 * Skips leading whitespace, then delegates to either
 * \c ft_handle_operator() or \c ft_handle_word() depending on
 * whether the current character is a meta-character.
 * \param l The lexer state.
 * \return Heap-allocated \c t_token, or TOKEN_EOF at end of input.
 */
t_token	*get_next_token(t_lexer *l)
{
	if (!l)
		return (NULL);
	while (l->input[l->pos] && ft_isspace(l->input[l->pos]))
		l->pos++;
	if (l->pos >= l->len)
		return (create_token(TOKEN_EOF, NULL));
	if (ft_ismeta(l->input[l->pos]))
		return (ft_handle_operator(l));
	else
		return (ft_handle_word(l));
}

/**
 * \brief Dispatch operator tokenisation for the current meta-character.
 * \param l The lexer state positioned at a meta-character.
 * \return The corresponding token, or TOKEN_ERROR for unsupported operators.
 */
t_token	*ft_handle_operator(t_lexer *l)
{
	char	current;

	current = l->input[l->pos];
	if (current == '|')
		return (ft_which_or(l));
	else if (current == '&')
		return (ft_which_and(l));
	else if (current == '<' || current == '>')
		return (ft_which_redir(l));
	else if (current == '(')
	{
		l->pos++;
		return (create_token(TOKEN_LEFT_PAR, ft_strdup("(")));
	}
	else if (current == ')')
	{
		l->pos++;
		return (create_token(TOKEN_RIGHT_PAR, ft_strdup(")")));
	}
	else
		return (create_token(TOKEN_ERROR, ft_strdup("Unsuported operator")));
}

/**
 * \brief Disambiguate `|` (pipe) from `||` (logical OR).
 * \param l The lexer state positioned at `|`.
 * \return TOKEN_OR if the next character is also `|`; otherwise TOKEN_PIPE.
 */
t_token	*ft_which_or(t_lexer *l)
{
	if (l->pos + 1 < l->len && l->input[l->pos + 1] == '|')
	{
		l->pos += 2;
		return (create_token(TOKEN_OR, ft_strdup("||")));
	}
	else
	{
		l->pos++;
		return (create_token(TOKEN_PIPE, ft_strdup("|")));
	}
}
