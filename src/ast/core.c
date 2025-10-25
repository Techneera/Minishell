#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast	*ast_root;

	parser = ft_init_parser(l);
	if (!parser)
	{
		fprintf("Error parser allocate.");
		return (NULL);
	}
	ast_root = ft_parse_and_or(parser); // Search lowest precedence
	if (ast_root != NULL && ast_root->current->tok_label == TOKEN_EOF)
	{
		fprintf("Error creating tree.");
		ft_free_ast(ast_root);
		ast_root = NULL;
	}
	free(parser->current_cmd);
	free(parser->peek);
	free(parser);
	return (ast_root);
}

/*
t_ast	*ft_parse_and_or(t_parser *parser)
{

}
*/
