/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPANSION_H
# define EXPANSION_H

# include "libshell.h"
# include "execution.h"
# include "ast.h"
# include "lexer.h"

/**
 * \brief Expansion finite-state-machine states.
 *
 * Controls how characters are interpreted inside \c expand_word().
 */
typedef enum s_exp_state
{
	EXP_GENERAL, /**< Normal mode: expand `$`, detect quote transitions. */
	EXP_SQUOTE,  /**< Inside single quotes: copy literally, no expansion. */
	EXP_DQUOTE   /**< Inside double quotes: expand `$` but suppress other metas. */
}	t_exp_state;

/**
 * \brief Context for one word-expansion operation.
 *
 * Accumulates the expanded result character by character.
 */
typedef struct s_exp_ctx
{
	char		*final;  /**< The result string being assembled (heap-allocated). */
	char		*raw;    /**< A copy of the raw input string being expanded. */
	char		**envp;  /**< The environment array used for variable lookup. */
	int			status;  /**< Last exit status, used for `$?` expansion. */
	int			i;       /**< Current read position in \c raw. */
	int			start;   /**< Start of the current literal segment. */
	t_exp_state	state;   /**< Current FSM state. */
}	t_exp_ctx;

/**
 * \brief Expand all variables and strip quotes from a raw word string.
 * \param raw_str The raw token string to expand (ownership transferred).
 * \param envp    The environment array.
 * \param status  The last exit status (for `$?`).
 * \return Heap-allocated expanded string, or NULL on failure.
 */
char	*expand_word(char *raw_str, char **envp, int status);

/**
 * \brief Resolve a `$...` sequence starting at \c s to its string value.
 * \param s      Pointer into the raw string at the `$` character.
 * \param envp   The environment array.
 * \param i      Current index into the raw string (advanced past the variable name).
 * \param status Last exit status (for `$?`).
 * \return Heap-allocated value string, or NULL on failure.
 */
char	*ft_get_expanded_value(char *s, char **envp, int *i, int status);

/**
 * \brief Join two strings, freeing \c s1 afterwards.
 * \param s1 The first string (freed after the join; may be NULL).
 * \param s2 The second string (not freed).
 * \return Heap-allocated concatenation, or NULL on failure.
 */
char	*ft_strjoin_free_s1(char *s1, char *s2);

/**
 * \brief Replace every `*` in a string with the control byte `\001`.
 *
 * Called before variable expansion so that literal asterisks inside
 * quotes survive and are not later treated as glob patterns.
 * \param str The string to mask in place.
 */
void	mask_wildcards(char *str);

/**
 * \brief Initialise an expansion context.
 * \param ctx The context struct to initialise.
 * \param raw The raw string to expand (will be stored in \c ctx->raw).
 * \param env The environment array.
 * \param st  The last exit status.
 */
void	ft_init_exp_ctx(t_exp_ctx *ctx, char *raw, char **env, int st);

/**
 * \brief Expand a `$VARNAME` sequence by looking up the environment.
 * \param s    Pointer into the raw string at `$`.
 * \param envp The environment array.
 * \param i    Index in the raw string (advanced past the variable name).
 * \return Heap-allocated value string, or empty string if not found.
 */
char	*ft_expand_env_var_aux(char *s, char **envp, int *i);

/**
 * \brief Process one character in EXP_GENERAL state.
 * \param ctx The expansion context.
 */
void	ft_handle_general(t_exp_ctx *ctx);

/**
 * \brief Process one character in EXP_SQUOTE state.
 * \param ctx The expansion context.
 */
void	ft_handle_squote(t_exp_ctx *ctx);

/**
 * \brief Process one character in EXP_DQUOTE state.
 * \param ctx The expansion context.
 */
void	ft_handle_dquote(t_exp_ctx *ctx);

/**
 * \brief Handle a `$` character encountered in EXP_GENERAL state.
 * \param ctx The expansion context.
 */
void	ft_general_dollar_aux(t_exp_ctx *ctx);

/**
 * \brief Append a literal character segment in EXP_GENERAL state.
 * \param ctx The expansion context.
 */
void	ft_general_char_aux(t_exp_ctx *ctx);

/**
 * \brief Test whether a string contains any single or double quote.
 * \param str The string to inspect.
 * \return Non-zero if a quote character is found; 0 otherwise.
 */
int		has_quotes(char *str);

/**
 * \brief Replace the empty-argument marker byte `\001` back with `\0`.
 *
 * After wildcard expansion, arguments that expanded to an empty quoted
 * string are marked with `\001` as their first byte. This restores them
 * to proper empty strings.
 * \param args The NULL-terminated argument array to process in place.
 */
void	ft_restore_markers(char **args);

/**
 * \brief Expand a single argument that contains `*` into matching filenames.
 * \param data The shell state (modified: \c tree->cmd->args is updated).
 * \param i    The index of the argument in the args array.
 * \param str  The argument string (must contain `*`).
 * \return The new index after the inserted matches, or -1 if no `*` or no matches.
 */
int		ft_handle_wildcard(t_data *data, int i, char *str);

/**
 * \brief Expand and possibly glob-expand one argument at index \c i.
 * \param data The shell state.
 * \param i    Index of the argument to process.
 * \return The next index to process (may skip multiple positions after glob expansion).
 */
int		ft_process_arg(t_data *data, int i);

/**
 * \brief Expand all arguments in \c data->tree->cmd->args in place.
 *
 * Calls \c expand_word() on each argument, handles wildcard expansion,
 * strips empty-argument markers, and shrinks the args array if needed.
 * \param data The shell state whose current command arguments are expanded.
 */
void	expand_args(t_data *data);
#endif
