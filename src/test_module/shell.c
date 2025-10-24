#include "libshell.h"
#include "lexer.h"

void	loop(void);
void	print_tokens(t_token *head);

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
		print_tokens(head);
		free_all(line, lex, &head);
	}
	free_all(line, NULL, &head);
}

void	print_tokens(t_token *head)
{
	t_token	*ptr;

	if (!head)
		return ;
	ptr = head;
	while (ptr)
	{
		printf("%d, %s\n", ptr->tok_label, ptr->str);
		ptr = ptr->next;
	}
}
