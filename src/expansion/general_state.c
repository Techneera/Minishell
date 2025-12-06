/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_state.c                                    :+:      :+:    :+:   */
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

void	ft_general_dollar_aux(t_exp_ctx *ctx)
{
	char	*chunk;

	chunk = ft_get_expanded_value(&ctx->raw[ctx->i],
			ctx->envp, &ctx->i, ctx->status);
	ctx->final = ft_strjoin_free_s1(ctx->final, chunk);
	free(chunk);
	ctx->start = ctx->i;
}

void	ft_general_char_aux(t_exp_ctx *ctx)
{
	char	tmp[2];

	tmp[0] = ctx->raw[ctx->i++];
	tmp[1] = 0;
	ctx->final = ft_strjoin_free_s1(ctx->final, tmp);
}

void	ft_handle_general(t_exp_ctx *ctx)
{
	if (ctx->raw[ctx->i] == '\'')
	{
		ctx->state = EXP_SQUOTE;
		ctx->start = ctx->i++ + 1;
	}
	else if (ctx->raw[ctx->i] == '"')
	{
		ctx->state = EXP_DQUOTE;
		ctx->start = ctx->i++ + 1;
	}
	else if (ctx->raw[ctx->i] == '$')
		ft_general_dollar_aux(ctx);
	else
		ft_general_char_aux(ctx);
}
