#include "libshell.h"
#include "lexer.h"

int	ft_ismeta(int c)
{
	return (c == '|' || c == '&' || c == '<' || \
c == '>' || c == '(' || c == ')');
}

t_lexer	*ft_state_lexer(char *line)
{
	t_lexer	*ptr;

	ptr = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!ptr)
		return (NULL);
	ptr->input = line;
	ptr->pos = 0;
	ptr->len = ft_strlen(line);
	return (ptr);
}

t_token	*get_next_token(t_lexer *l)
{
	if (!l)
		return (NULL);
	while (l->input[l->pos] && ft_isspace(l->input[l->pos]))
		l->pos++;
	if (l->pos >= l->len)
		return (create_token(TOKEN_EOF, NULL));
	if (ft_ismeta(l->input[l->pos]))
		return (ft_handle_operator(l));
	else
		return (ft_handle_word(l));
}

t_token	*ft_handle_operator(t_lexer *l)
{
	char	current;

	current = l->input[l->pos];
	if (current == '|')
		return (ft_which_or(l));
	else if (current == '&')
		return (ft_which_and(l));
	else if (current == '<' || current == '>')
		return (ft_which_redir(l));
	else if (current == '(')
	{
		l->pos++;
		return (create_token(TOKEN_LEFT_PAR, ft_strdup("(")));
	}
	else if (current == ')')
	{
		l->pos++;
		return (create_token(TOKEN_RIGHT_PAR, ft_strdup(")")));
	}
	else
		return (create_token(TOKEN_ERROR, ft_strdup("Unsuported operator")));
}

t_token	*ft_which_or(t_lexer *l)
{
	if (l->pos + 1 < l->len && l->input[l->pos + 1] == '|')
	{
		l->pos += 2;
		return (create_token(TOKEN_OR, ft_strdup("||")));
	}
	else
	{
		l->pos++;
		return (create_token(TOKEN_PIPE, ft_strdup("|")));
	}
}
