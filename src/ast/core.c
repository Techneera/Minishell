#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast		*ast_root;
	t_parser	*parser;

	parser = ft_init_parser(l);
	if (!parser)
		return (ft_putstr_fd("Error parser allocate.\n", 2), NULL);
	ast_root = ft_parse_and_or(parser);
	if (ast_root != NULL && parser->current_token->tok_label != TOKEN_EOF)
	{
		ft_putstr_fd("Syntax error near unexpected token: ", 2);
		ft_putstr_fd(parser->current_token->str, 2);
		ft_putstr_fd(".\n", 2);
		ft_free_ast(ast_root);
		ast_root = NULL;
	}
	free_token(parser->current_token);
	free_token(parser->peek);
	free(parser);
	return (ast_root);
}

static
void	ft_which_logic_op(t_node_type *op, t_parser *parser)
{
	if (parser->current_token->tok_label == TOKEN_AND)
		*op = NODE_AND;
	else
		*op = NODE_OR;
}

t_ast	*ft_parse_and_or(t_parser *parser)
{
	t_ast		*node;
	t_ast		*l_node;
	t_ast		*r_node;
	t_node_type	op;

	l_node = ft_parse_pipeline(parser);
	if (!l_node)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_AND || \
parser->current_token->tok_label == TOKEN_OR)
	{
		ft_which_logic_op(&op, parser);
		ft_parser_iter(parser);
		r_node = ft_parse_pipeline(parser);
		if (!r_node)
			return (ft_free_ast(l_node), NULL);
		node = ft_ast_generic_node(op);
		if (!node)
			return (ft_free_ast(l_node), ft_free_ast(r_node), NULL);
		node->left = l_node;
		node->right = r_node;
		l_node = node;
	}
	return (l_node);
}

t_ast	*ft_parse_pipeline(t_parser *parser)
{
	t_ast	*node;
	t_ast	*r_node;
	t_ast	*new_p;

	node = ft_parse_grain_with_redirs(parser);
	if (!node)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_PIPE)
	{
		ft_parser_iter(parser);
		r_node = ft_parse_grain_with_redirs(parser);
		if (!r_node)
			return (ft_free_ast(node), NULL);
		new_p = ft_ast_generic_node(NODE_PIPE);
		if (!new_p)
			return (ft_free_ast(node), ft_free_ast(r_node), NULL);
		new_p->left = node;
		new_p->right = r_node;
		node = new_p;
	}
	return (node);
}

t_ast	*ft_parse_grain_with_redirs(t_parser *parser)
{
	t_ast	*node;

	node = NULL;
	if (parser->current_token->tok_label == TOKEN_LEFT_PAR)
		node = ft_parse_subshell(parser);
	else if (parser->current_token->tok_label == TOKEN_WORD || \
ft_isredir(parser))
		node = ft_parse_simple_command(parser);
	else
		ft_putstr_fd("Error invalid token.\n", 2);
	if (!node)
		return (NULL);
	if (node->type == NODE_SUBSHELL)
	{
		node->cmd = ft_create_command(NULL, NULL, 0);
		if (!node->cmd)
			return (ft_free_ast(node), NULL);
		if (ft_handle_redirects(parser, node->cmd) == false)
			return (ft_free_ast(node), NULL);
	}
	return (node);
}
