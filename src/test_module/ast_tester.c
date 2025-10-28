#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include <assert.h>

static
void	test_parser(char *input)
{
	t_lexer		*lexer;
	t_parser	*parser;

	lexer = ft_state_lexer(input);
	if (!lexer)
	{
		fprintf(stderr, "Error allocating lexer");
		return ;
	}
	parser = ft_init_parser(lexer);
	if (!parser)
	{
		fprintf(stderr, "Error parser");
		free_lexer(lexer);
		return ;
	}
	while (parser->current_token->tok_label != TOKEN_EOF)
	{
		ft_printf("%d ,", parser->current_token->tok_label);
		ft_printf("%s\n", parser->current_token->str);
		ft_parser_iter(parser);
	}
	free(parser->lex);
	free_token(parser->current_token);
	free_token(parser->peek);
	free(parser);
}

void	test_handle_and_or(char *input)
{
	t_lexer		*lexer;
	t_ast		*root;

	lexer = ft_state_lexer(input);
	if (!lexer)
	{
		fprintf(stderr, "Error allocating lexer");
		return ;
	}
	root = ft_parser(lexer);
}

int	main(void)
{
	test_parser("echo fa\"$HOME\" | grep a");
    return (0);
}
