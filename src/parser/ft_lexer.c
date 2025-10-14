#include "libshell.h"

static
int	ft_ismeta(int c)
{
    return (c == '|' | c == '&' | c == '<' | c == '>' | c == '(' | c == ')');
}

t_lexer	*ft_state_lexer(const char *line)
{
    t_lexer	*ptr;

    ptr = (l_lexer *)ft_calloc(1, sizeof(t_lexer));
    if (!ptr)
	return (NULL);
    ptr->input = line;
    ptr->pos = 0;
    ptr->len = ft_strlen(line);
    return (ptr);
}

t_token	*get_next_token(t_lexer *l)
{
    char	*ptr;

    ptr = NULL;
    if (!l)
	return (NULL);
    ptr = l->input + l->pos;
    while (*ptr && ft_isspace(*ptr))
    {
	l->pos++;
	ptr++;
    }
    if (l->pos >= l->len)
	return (create_token(TOKEN_EOF, NULL));
    if (ft_ismeta(*ptr))
	return (ft_handle_operator(l));
    else
	return (ft_handle_word(l));
}

t_token	*ft_handle_operator(t_lexer *l)
{
    char	current;

    current = l->input[l->pos];
    if (current == '|')
	ft_which_or(l);
    else if (*ptr == '&')
	ft_which_and(l);
    else if (*ptr == '<' || *ptr == '>')
	ft_which_redir(l);
    else if (*ptr == '(')
    {
	l->pos++;
	return (create_token(TOKEN_LEFT_PAR, "("));
    }
    else if (*ptr == ')')
    {
	l->pos++;
	return (create_token(TOKEN_RIGHT_PAR, ")"));
    }
    else
	return (create_token(TOKEN_ERROR, "Unsuported operator"));
}

void	ft_which_or(t_lexer *l)
{
    if (l->pos + 1 < l->len && *(ptr + 1) == '|')
    {
	tok = create_token(TOKEN_OR, "||");
	l->pos += 2;
    }
    else
    {
	tok = create_token(TOKEN_PIPE, "|");
	l->pos++;
    }
}

void	ft_which_and(t_lexer *l)
{
    if (l->pos + 1 < l->len && *(ptr + 1) == '&')
    {
	tok = create_token(TOKEN_AND, "&&");
	l->pos += 2;
    }
}

void	ft_which_redir(t_lexer *l)
{
    char	*delim;

    if (l->pos + 1 < l->len && *(ptr + 1) == '>')
    {
	l->pos += 2;
	return create_token(TOKEN_REDIR_APPEND, ">>"));
    }
    else if (l->pos + 1 < l->len && *(ptr + 1) == '<')
    {
	l->pos += 2;
	while (l->pos < l->len && ft_isspace(l->input[l->pos]))
	    l->pos++;
	delim = ft_get_unquoted_str(l->input[l->pos]);
	if (!delim)
	    tok = create_token(TOKEN_ERROR, "minishell: syntax error near unexpected token `<'");
	tok = create_token(TOKEN_REDIR_HEREDOC, delim);
    }
    else if (*ptr == "<")
    {
	tok = create_token(TOKEN_REDIR_IN, "<");
	l->pos++;
    }
    else
	return ;
}
/*
t_token	*ft_handle_word()
*/
