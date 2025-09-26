
#include "libshell.h"

t_token	*create_token(char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = str;
	token->next = NULL;
	token->previus = NULL;
	return (token);	
}

int	token_add_back(t_token **token, char *str)
{
	t_token	*token_tmp;
	
	if (!(*token))
	{
		*token = create_token(str);
		if (!(*token))
			return (-1);
		return (0);		
	}
	token_tmp = *token;
	while (token_tmp->next)
		token_tmp = token_tmp->next;
	token_tmp->next = create_token(str);
	if (!token_tmp->next)
		return (-1);
	token_tmp->next->previus = token_tmp;
	return (0);
}

void	ft_free_token(t_token **token)
{
	t_token	*tmp;

	tmp = (*token)->next;
	while (tmp)
	{
		free(*token);
		*token = NULL;
		*token = tmp;
		tmp = tmp->next;
	}
	if (*token)
		free(*token);
	*token = NULL;	
}

void	token_print(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
}