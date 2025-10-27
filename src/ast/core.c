#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast		*ast_root;
	t_parser	*parser;

	parser = ft_init_parser(l);
	if (!parser)
	{
		fprintf(stderr, "Error parser allocate.");
		return (NULL);
	}
	ast_root = ft_parse_and_or(parser); // Search lowest precedence
	if (ast_root != NULL && parser->current_token->tok_label == TOKEN_EOF)
	{
		fprintf(stderr, "Error creating tree.");
		ft_free_ast(ast_root);
		ast_root = NULL;
	}
	free(parser->current_token);
	free(parser->peek);
	free(parser);
	return (ast_root);
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
		if (parser->current_token->tok_label == TOKEN_AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;
		ft_parser_iter(parser);
		ast_right = ft_parse_pipeline(parser);
		if (!ast_right)
		{
			ft_free_ast(ast_left);
			return (NULL);
		}
		new_parent = ft_ast_generic_node(op_type);
		if (!new_parent)
		{
			ft_free_ast(ast_left);
			ft_free_ast(ast_right);
			return (NULL);
		}
		new_parent->left = ast_left;
		new_parent->right= ast_right;
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
			return (NULL);
		new_parent = ft_ast_generic_node(NODE_PIPE);
		if (!new_parent)
			return (NULL);
		new_parent->left = ast_left;
		new_parent->right= ast_right;
		ast_left = new_parent;
	}
	return (ast_left);
}
/*
t_ast	*ft_parse_grain_with_redirs(t_parser *parser)
{
	t_ast	*
}
*/
