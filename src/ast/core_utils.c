/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
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

void	ft_init_simp_ctx(t_simp_ctx *ctx)
{
	ctx->args = NULL;
	ctx->redirs = NULL;
	ctx->cmd = NULL;
	ctx->node = NULL;
	ctx->tmp_redir = NULL;
}

void	*ft_free_simp_ctx(t_simp_ctx *ctx)
{
	if (ctx->args)
		ft_lstclear(&ctx->args, &free);
	if (ctx->redirs)
		ft_lstclear(&ctx->redirs, &ft_free_redir_content);
	if (ctx->cmd)
		ft_free_cmd(ctx->cmd);
	return (NULL);
}

t_ast	*ft_parse_simple_command(t_parser *parser)
{
	t_simp_ctx	ctx;

	ft_init_simp_ctx(&ctx);
	while (parser->current_token->tok_label == TOKEN_WORD || ft_isredir(parser))
	{
		if (parser->current_token->tok_label == TOKEN_WORD)
		{
			if (!ft_handle_word_token(parser, &ctx))
				return (ft_free_simp_ctx(&ctx));
		}
		else if (ft_isredir(parser))
		{
			if (!ft_handle_redir_token(parser, &ctx))
				return (ft_free_simp_ctx(&ctx));
		}
	}
	return (ft_finalize_simp_cmd(&ctx));
}

int	ft_handle_word_token(t_parser *p, t_simp_ctx *ctx)
{
	char	*str;
	t_list	*new_node;

	str = ft_strdup(p->current_token->str);
	if (!str)
		return (0);
	new_node = ft_lstnew(str);
	if (!new_node)
	{
		free(str);
		return (0);
	}
	ft_lstadd_back(&ctx->args, new_node);
	ft_parser_iter(p);
	return (1);
}

int	ft_handle_redir_token(t_parser *p, t_simp_ctx *ctx)
{
	t_redir	*redir;
	t_list	*new_node;

	redir = ft_parse_single_redir(p);
	if (!redir)
		return (0);
	new_node = ft_lstnew(redir);
	if (!new_node)
	{
		ft_free_redir_content(redir);
		return (0);
	}
	ft_lstadd_back(&ctx->redirs, new_node);
	return (1);
}
