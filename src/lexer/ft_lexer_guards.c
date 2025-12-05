#include "libshell.h"
#include "lexer.h"

void	free_token_lst(t_token **tok_lst, void (*del)(t_token *))
{
	t_token	*ptr;

	if (!tok_lst)
		return ;
	while (*tok_lst)
	{
		ptr = (*tok_lst)->next;
		del(*tok_lst);
		*tok_lst = ptr;
	}
	*tok_lst = NULL;
}

void	free_token(t_token *t)
{
	if (!t)
		return ;
	free(t->str);
	free(t);
	t = NULL;
}

void	free_lexer(t_lexer *l)
{
	if (!l)
		return ;
	free(l);
	l = NULL;
}

void	ft_tokens_constructor(t_lexer **lex, t_token **head)
{
	t_token	*tmp;

	tmp = get_next_token(*lex);
	if (!tmp)
		return ;
	while (tmp->tok_label != TOKEN_EOF && tmp->tok_label != TOKEN_ERROR)
	{
		add_token_back(head, tmp);
		tmp = get_next_token(*lex);
	}
	add_token_back(head, tmp);
}
