#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include <assert.h>

# define ANSI_COLOR_RED     "\x1b[31m"
# define ANSI_COLOR_GREEN   "\x1b[32m"
# define ANSI_COLOR_YELLOW  "\x1b[33m"
# define ANSI_COLOR_BLUE    "\x1b[34m"
# define ANSI_COLOR_MAGENTA "\x1b[35m"
# define ANSI_COLOR_CYAN    "\x1b[36m"
# define ANSI_COLOR_RESET   "\x1b[0m"

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
	free(parser);
}

int	main(void)
{
	test_parser("ls -l | grep a");
    return (0);
}
