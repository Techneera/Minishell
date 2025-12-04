#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_parse_grain_with_redirs(t_parser *parser)
{
	t_ast	*node;
	t_cmd	*cmd_to_fill;

	node = NULL;
	if (parser->current_token->tok_label == TOKEN_LEFT_PAR)
		node = ft_parse_subshell(parser);
	else if (parser->current_token->tok_label == TOKEN_WORD \
|| ft_isredir(parser))
		node = ft_parse_simple_command(parser);
	else
		fprintf(stderr, "Error invalid token.\n");
	if (!node)
		return (NULL);
	if (node->type == NODE_SUBSHELL)
	{
		node->cmd = ft_create_command(NULL, NULL, 0);
		if (!node->cmd)
			return (ft_free_ast(node), NULL);
		cmd_to_fill = node->cmd;
		if (ft_handle_redirects(parser, cmd_to_fill) == false)
			return (ft_free_ast(node), NULL);
	}
	return (node);
}

void	ft_parse_args(t_parser *parser, t_list **head)
{
	char	*arg_cpy;

	arg_cpy = strdup(parser->current_token->str);
	if (!arg_cpy)
	{
		ft_lstclear(head, &free);
		return ;
	}
	ft_lstadd_back(head, ft_lstnew(arg_cpy));
	ft_parser_iter(parser);
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

char	**ft_lst_to_args(t_list **head, int size)
{
	char	**ret;
	t_list	*tmp;
	int		i;

	ret = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (*head)
	{
		tmp = *head;
		ret[i] = (char *)tmp->content;
		*head = tmp->next;
		free(tmp);
		i++;
	}
	return (ret);
}

t_ast	*ft_parse_subshell(t_parser *parser)
{
	t_ast	*node_subsh;
	t_ast	*body;

	if (parser->peek->tok_label == TOKEN_RIGHT_PAR)
	{
		ft_parser_iter(parser);
		ft_parser_iter(parser);
		return (fprintf(stderr, "Syntax error near token \")\".\n"), NULL);
	}
	ft_parser_iter(parser);
	body = ft_parse_and_or(parser);
	if (!body)
		return (fprintf(stderr, "Error in subshell body.\n"), NULL);
	if (parser->current_token->tok_label != TOKEN_RIGHT_PAR)
		return (fprintf(stderr, "Error unclosed parenthese.\n"), \
ft_free_ast(body), NULL);
	ft_parser_iter(parser);
	node_subsh = ft_ast_generic_node(NODE_SUBSHELL);
	if (!node_subsh)
		return (ft_free_ast(body), NULL);
	node_subsh->body = body;
	return (node_subsh);
}
