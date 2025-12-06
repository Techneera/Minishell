/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
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

char	*ft_expand_env_var_aux(char *s, char **envp, int *i)
{
	int		len;
	char	*name;
	char	*val;

	len = 1;
	while (ft_isalnum(s[len]) || s[len] == '_')
		len++;
	if (len == 1)
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	name = ft_substr(s, 1, len - 1);
	val = ft_getenv(envp, name);
	free(name);
	*i += len;
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

void	ft_handle_squote(t_exp_ctx *ctx)
{
	char	*chunk;

	if (ctx->raw[ctx->i] == '\'')
	{
		ctx->state = EXP_GENERAL;
		chunk = ft_substr(ctx->raw, ctx->start, ctx->i - ctx->start);
		mask_wildcards(chunk);
		ctx->final = ft_strjoin_free_s1(ctx->final, chunk);
		free(chunk);
		ctx->i++;
	}
	else
		ctx->i++;
}

void	ft_dquote_dollar_aux(t_exp_ctx *ctx)
{
	char	*chunk;

	chunk = ft_substr(ctx->raw, ctx->start, ctx->i - ctx->start);
	mask_wildcards(chunk);
	ctx->final = ft_strjoin_free_s1(ctx->final, chunk);
	free(chunk);
	chunk = ft_get_expanded_value(&ctx->raw[ctx->i],
			ctx->envp, &ctx->i, ctx->status);
	ctx->final = ft_strjoin_free_s1(ctx->final, chunk);
	free(chunk);
	ctx->start = ctx->i;
}

void	ft_handle_dquote(t_exp_ctx *ctx)
{
	char	*chunk;

	if (ctx->raw[ctx->i] == '"')
	{
		ctx->state = EXP_GENERAL;
		chunk = ft_substr(ctx->raw, ctx->start, ctx->i - ctx->start);
		mask_wildcards(chunk);
		ctx->final = ft_strjoin_free_s1(ctx->final, chunk);
		free(chunk);
		ctx->i++;
	}
	else if (ctx->raw[ctx->i] == '$')
		ft_dquote_dollar_aux(ctx);
	else
		ctx->i++;
}
