#include "libshell.h"
#include "lexer.h"

void	loop(void);
void	ft_tokens_constructor(t_lexer **lex, t_token **head);
void	free_all(char *line, t_lexer *lex, t_token **head);

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	loop();
	return (0);
}

void	loop(void)
{
	char	*line;
	t_lexer	*lex;
	t_token	*head;

	line = NULL;
	lex = NULL;
	while(1)
	{
		head = NULL;
		line = readline(PROMPT);
		if (!line || !ft_strcmp(line, "exit"))
			break ;
		lex = ft_state_lexer(line);
		if (!lex)
		{
			free(line);
			break ;
		}
		ft_tokens_constructor(&lex, &head);
		free_all(line, lex, &head);
	}
	free_all(line, NULL, &head);
}

void	free_all(char *line, t_lexer *lex, t_token **head)
{
	if (line)
	{
		free(line);
		line = NULL;
	}
	if (lex)
	{
		free_lexer(lex);
		lex = NULL;
	}
	if (head)
	{
		free_token_lst(head, &free_token);
		head = NULL;
	}
}

void	ft_tokens_constructor(t_lexer **lex, t_token **head)
{
	t_token	*tmp;

	tmp = get_next_token(*lex);
	if (!tmp)
		return ;
	while(tmp->tok_label != TOKEN_EOF && tmp->tok_label != TOKEN_ERROR)
	{
		add_token_back(head, tmp);
		tmp = get_next_token(*lex);
	}
	add_token_back(head, tmp);
}
