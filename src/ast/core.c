#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast		*ast_root;
	t_parser	*parser;

	parser = ft_init_parser(l);
	if (!parser)
	{
		fprintf(stderr, "Error parser allocate.\n");
		return (NULL);
	}
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

static
void	ft_operator_type(t_parser *parser, t_node_type *op_type)
{
	if (parser->current_token->tok_label == TOKEN_AND)
		*op_type = NODE_AND;
	else
		*op_type = NODE_OR;
}

t_ast	*ft_parse_and_or(t_parser *parser)
{
	t_ast		*new_parent;
	t_ast		*ast_left;
	t_ast		*ast_right;
	t_node_type	op_type;

	ast_left = ft_parse_pipeline(parser);
	if (!ast_left)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_AND || \
parser->current_token->tok_label == TOKEN_OR)
	{
		ft_operator_type(parser, &op_type);
		ft_parser_iter(parser);
		ast_right = ft_parse_pipeline(parser);
		if (!ast_right)
			return (ft_free_ast(ast_left), NULL);
		new_parent = ft_ast_generic_node(op_type);
		if (!new_parent)
			return (ft_free_ast(ast_left), ft_free_ast(ast_right), NULL);
		new_parent->left = ast_left;
		new_parent->right = ast_right;
		ast_left = new_parent;
	}
	return (ast_left);
}

t_ast	*ft_parse_pipeline(t_parser *parser)
{
	t_ast	*new_parent;
	t_ast	*ast_left;
	t_ast	*ast_right;

	ast_left = ft_parse_grain_with_redirs(parser);
	if (!ast_left)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_PIPE)
	{
		ft_parser_iter(parser);
		ast_right = ft_parse_grain_with_redirs(parser);
		if (!ast_right)
			return (ft_free_ast(ast_left), NULL);
		new_parent = ft_ast_generic_node(NODE_PIPE);
		if (!new_parent)
			return (ft_free_ast(ast_left), ft_free_ast(ast_right), NULL);
		new_parent->left = ast_left;
		new_parent->right = ast_right;
		ast_left = new_parent;
	}
	return (ast_left);
}
