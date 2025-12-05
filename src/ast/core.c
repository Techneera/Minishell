#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast		*ast_root;
	t_parser	*parser;

	parser = ft_init_parser(l);
	if (!parser)
		return (fprintf(stderr, "Error parser allocate.\n"), NULL);
	ast_root = ft_parse_and_or(parser);
	if (ast_root != NULL && parser->current_token->tok_label != TOKEN_EOF)
	{
		fprintf(stderr, "Syntax error near unexpected token: %s.\n", \
parser->current_token->str);
		ft_free_ast(ast_root);
		ast_root = NULL;
	}
	free_token(parser->current_token);
	free_token(parser->peek);
	free(parser);
	return (ast_root);
}

t_ast	*ft_parse_and_or(t_parser *parser)
{
	t_ast		*node;
	t_ast		*r_node;
	t_node_type	op;

	node = ft_parse_pipeline(parser);
	if (!node)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_AND || \
parser->current_token->tok_label == TOKEN_OR)
	{
		if (parser->current_token->tok_label == TOKEN_AND)
			op = NODE_AND;
		else
			op = NODE_OR;
		ft_parser_iter(parser);
		r_node = ft_parse_pipeline(parser);
		if (!r_node)
			return (ft_free_ast(node), NULL);
		node = ft_ast_generic_node(op);
		if (!node)
			return (ft_free_ast(node), ft_free_ast(r_node), NULL);
		node->left = node;
		node->right = r_node;
	}
	return (node);
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
		fprintf(stderr, "Error invalid token.\n");
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
