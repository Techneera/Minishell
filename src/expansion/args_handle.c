/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expansion.h"
#include "libft.h"
#include "execution.h"

/**
 * \brief Replace all `*` characters in a string with the control byte `\001`.
 *
 * Must be called before variable expansion so that literal asterisks
 * inside quotes survive and are not later treated as glob patterns.
 * \param str The string to mask in place (safe to call with NULL).
 */
void	mask_wildcards(char *str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			str[i] = '\001';
		i++;
	}
}

/**
 * \brief Join two strings and free \c s1 immediately afterwards.
 * \param s1 First string (freed after join; NULL is treated as "").
 * \param s2 Second string (not freed).
 * \return Heap-allocated concatenation, or NULL on failure.
 */
char	*ft_strjoin_free_s1(char *s1, char *s2)
{
	char	*new_str;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

/**
 * \brief Resolve a `$...` sequence to its expanded string value.
 *
 * Handles three cases in order:
 * - `$?`  → converts the last exit status to a decimal string.
 * - `$n`  → positional parameter (returns empty string; not supported).
 * - `$ID` → looks up the variable name in \c envp.
 * \param s      Pointer into the raw string at the `$` character.
 * \param envp   The environment array.
 * \param i      Read index (advanced past the consumed variable name).
 * \param status The last exit status (for `$?`).
 * \return Heap-allocated value string, or NULL on failure.
 */
char	*ft_get_expanded_value(char *s, char **envp, int *i, int status)
{
	if (s[1] == '?')
	{
		*i += 2;
		return (ft_itoa(status));
	}
	if (ft_isdigit(s[1]))
	{
		*i += 2;
		return (ft_strdup(""));
	}
	return (ft_expand_env_var_aux(s, envp, i));
}

/**
 * \brief Expand all variables and strip quotes from a raw word string.
 *
 * Drives the FSM through EXP_GENERAL / EXP_SQUOTE / EXP_DQUOTE states.
 * The raw string (\c raw_str) is duplicated into \c ctx.raw and freed
 * at the end.
 * \param raw_str The raw token string (ownership transferred).
 * \param envp    The environment array.
 * \param status  The last exit status (for `$?`).
 * \return Heap-allocated expanded string, or NULL on failure.
 */
char	*expand_word(char *raw_str, char **envp, int status)
{
	t_exp_ctx	ctx;

	ft_init_exp_ctx(&ctx, raw_str, envp, status);
	while (ctx.raw[ctx.i])
	{
		if (ctx.state == EXP_GENERAL)
			ft_handle_general(&ctx);
		else if (ctx.state == EXP_SQUOTE)
			ft_handle_squote(&ctx);
		else if (ctx.state == EXP_DQUOTE)
			ft_handle_dquote(&ctx);
	}
	if (ctx.raw)
		free(ctx.raw);
	return (ctx.final);
}

/**
 * \brief Initialise all fields of an expansion context.
 * \param ctx The context to initialise.
 * \param raw The raw input string to expand (stored as \c ctx->raw).
 * \param env The environment array.
 * \param st  The last exit status.
 */
void	ft_init_exp_ctx(t_exp_ctx *ctx, char *raw, char **env, int st)
{
	ctx->raw = raw;
	ctx->envp = env;
	ctx->status = st;
	ctx->final = ft_strdup("");
	ctx->i = 0;
	ctx->start = 0;
	ctx->state = EXP_GENERAL;
}
