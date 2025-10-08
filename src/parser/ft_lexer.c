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
	return (ft_handle_operator(ptr));
    else
	return (ft_handle_word(ptr));
}

/*
t_token	*ft_handle_token()
{
}

t_token	*ft_handle_word()
*/
