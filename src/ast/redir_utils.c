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
#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_finalize_simp_cmd(t_simp_ctx *ctx)
{
	ctx->cmd = ft_create_command(NULL, NULL, 0);
	if (!ctx->cmd)
		return (ft_free_simp_ctx(ctx));
	ctx->cmd->args = ft_lst_to_args(&ctx->args, ft_lstsize(ctx->args));
	ctx->cmd->redir_count = ft_lstsize(ctx->redirs);
	if (ctx->cmd->redir_count > 0)
	{
		ctx->cmd->redirs = ft_calloc(ctx->cmd->redir_count, sizeof(t_redir));
		if (!ctx->cmd->redirs)
			return (ft_free_simp_ctx(ctx));
		ft_copy_lst_to_array(ctx->redirs, ctx->cmd->redirs);
		ft_lstclear(&ctx->redirs, &ft_free_redir_struct_only);
	}
	ctx->node = ft_ast_node_command(ctx->cmd);
	if (!ctx->node)
		return (ft_free_simp_ctx(ctx));
	return (ctx->node);
}

t_label_redir	ft_label_map(t_token_label label)
{
	if (label == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (label == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (label == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (label == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOCK);
	return (REDIR_NONE);
}

t_redir	*ft_parse_single_redir(t_parser *parser)
{
	t_label_redir	label;
	char			*filename;
	t_redir			*redir;

	label = ft_label_map(parser->current_token->tok_label);
	if (label == REDIR_HEREDOCK)
	{
		filename = ft_strdup(parser->current_token->str);
		ft_parser_iter(parser);
	}
	else
	{
		if (parser->peek->tok_label != TOKEN_WORD)
			return (ft_putstr_fd("Unexpected token.\n", 2), NULL);
		filename = ft_strdup(parser->peek->str);
		ft_parser_iter(parser);
		ft_parser_iter(parser);
	}
	if (!filename)
		return (NULL);
	redir = ft_create_redir(label, filename);
	return (redir);
}

int	ft_handle_redirects(t_parser *parser, t_cmd *cmd_to_fill)
{
	t_list	*head;

	head = NULL;
	while (ft_isredir(parser))
	{
		if (!ft_proc_redir_loop(parser, &head, cmd_to_fill))
			return (ft_lstclear(&head, &ft_free_redir_content), false);
	}
	if (cmd_to_fill->redir_count > 0)
	{
		cmd_to_fill->redirs = ft_calloc(cmd_to_fill->redir_count, \
sizeof(t_redir));
		if (!cmd_to_fill->redirs)
			return (ft_lstclear(&head, &ft_free_redir_content), false);
		ft_copy_lst_to_array(head, cmd_to_fill->redirs);
		ft_lstclear(&head, &ft_free_redir_struct_only);
	}
	return (true);
}

t_redir	*ft_extract_redir_info(t_parser *p, t_label_redir label)
{
	char	*filename;

	if (label == REDIR_HEREDOCK)
		filename = ft_strdup(p->current_token->str);
	else
	{
		if (p->peek->tok_label != TOKEN_WORD)
			return (NULL);
		filename = ft_strdup(p->peek->str);
	}
	if (!filename)
		return (NULL);
	return (ft_create_redir(label, filename));
}
