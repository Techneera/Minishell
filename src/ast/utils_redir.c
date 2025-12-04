#include "libshell.h"
#include "ast.h"
#include "lexer.h"

static
int	ft_we(t_parser *parser, t_cmd *cmd_to_fill, t_wrapper *w)
{
	if (w->label == REDIR_HEREDOCK)
		w->filename = ft_strdup(parser->current_token->str);
	else
	{
		if (parser->peek->tok_label != TOKEN_WORD)
			return (ft_lstclear(&w->tmp_lst_head, &ft_free_redir_content), \
false);
		w->filename = ft_strdup(parser->peek->str);
	}
	if (!w->filename)
		return (ft_lstclear(&w->tmp_lst_head, &ft_free_redir_content), false);
	w->new_redir = ft_create_redir(w->label, w->filename);
	if (!w->new_redir)
		return (ft_lstclear(&w->tmp_lst_head, &ft_free_redir_content), false);
	w->new_link = ft_lstnew(w->new_redir);
	if (!w->new_link)
		return (ft_free_redir_content(w->new_redir), \
ft_lstclear(&w->tmp_lst_head, &ft_free_redir_content), false);
	ft_lstadd_back(&w->tmp_lst_head, w->new_link);
	cmd_to_fill->redir_count++;
	ft_parser_iter(parser);
	if (w->label != REDIR_HEREDOCK)
		ft_parser_iter(parser);
	return (true);
}

int	ft_isredir(t_parser *parser)
{
	return (parser->current_token->tok_label == TOKEN_REDIR_IN || \
parser->current_token->tok_label == TOKEN_REDIR_OUT || \
parser->current_token->tok_label == TOKEN_REDIR_APPEND || \
parser->current_token->tok_label == TOKEN_REDIR_HEREDOC);
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

int	ft_handle_redirects(t_parser *parser, t_cmd *cmd_to_fill)
{
	t_wrapper	w;

	w.tmp_lst_head = NULL;
	while (ft_isredir(parser))
	{
		w.label = ft_label_map(parser->current_token->tok_label);
		if (!ft_we(parser, cmd_to_fill, &w))
			return (false);
	}
	if (cmd_to_fill->redir_count > 0)
	{
		cmd_to_fill->redirs = (t_redir *)ft_calloc(cmd_to_fill->redir_count, \
sizeof(t_redir));
		if (!cmd_to_fill->redirs)
			return (ft_lstclear(&w.tmp_lst_head, &ft_free_redir_content), \
false);
		ft_copy_lst_to_array(w.tmp_lst_head, cmd_to_fill->redirs);
		ft_lstclear(&w.tmp_lst_head, &ft_free_redir_struct_only);
	}
	return (true);
}
