#include "libshell.h"
#include "lexer.h"

static
int	ft_ismeta(int c)
{
    return (c == '|' || c == '&' || c == '<' ||  c == '>' ||  c == '(' ||  c == ')');
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

t_token	*ft_which_and(t_lexer *l)
{
    if (l->pos + 1 < l->len && l->input[l->pos + 1] == '&')
    {
	l->pos += 2;
	return (create_token(TOKEN_AND, ft_strdup("&&")));
    }
    l->pos++;
    return (create_token(TOKEN_ERROR, ft_strdup("Unsuported operator")));
}

t_token	*ft_which_redir(t_lexer *l)
{
    char	*delim;

    if (l->pos + 1 < l->len && l->input[l->pos + 1] == '>')
    {
	l->pos += 2;
	return (create_token(TOKEN_REDIR_APPEND, ft_strdup(">>")));
    }
    else if (l->input[l->pos] == '>')
    {
	l->pos++;
	return (create_token(TOKEN_REDIR_OUT, ft_strdup(">")));
    }
    else if (l->pos + 1 < l->len && l->input[l->pos + 1] == '<')
    {
	l->pos += 2;
	while (l->pos < l->len && ft_isspace(l->input[l->pos]))
	    l->pos++;
	delim = ft_get_unquoted_str(l);
	if (!delim)
	    return (create_token(TOKEN_ERROR, ft_strdup("Unexpected error in delim.")));
	return (create_token(TOKEN_REDIR_HEREDOC, delim));
    }
    else if (l->input[l->pos] == '<')
    {
	l->pos++;
	return (create_token(TOKEN_REDIR_IN, ft_strdup("<")));
    }
    else
	return (create_token(TOKEN_ERROR, ft_strdup("minishell: syntax error near unexpected token `<'")));
}

t_token	*ft_handle_word(t_lexer *l)
{
    int		start;
    char	in_quotes;

    start = l->pos;
    in_quotes = '\0';
    while(l->pos < l->len)
    {
	if (in_quotes)
	{
	    if (l->input[l->pos] == in_quotes)
		in_quotes = '\0';
	}
	else
	{
	    if (l->input[l->pos] == '\'' || l->input[l->pos] == '\"')
		in_quotes = l->input[l->pos];
	    else if (ft_ismeta(l->input[l->pos]) || l->input[l->pos] == ' ')
		    break;
	}
	l->pos++;
    }
    if (in_quotes)
	return (create_token(TOKEN_ERROR, ft_strdup("Unclosed word.")));
    else
	return (create_token(TOKEN_WORD, ft_substr(l->input, start, l->pos - start)));
}

t_token	*create_token(t_token_label tok_label, char *str)
{
    t_token	*t;

    t = (t_token *)ft_calloc(1, sizeof(t_token));
    if (!t)
    {
        free(str);
	return (NULL);
    }
    t->str = str;
    t->tok_label = tok_label;
    t->next = NULL;
    return (t);
}

void	add_token_back(t_token **tok_lst, t_token *new)
{
    t_token	*ptr;

    if (!*tok_lst && new)
    {
	*tok_lst = new;
	return ;
    }
    ptr = *tok_lst;
    if (!new)
	return ;
    while (ptr->next)
	ptr = ptr->next;
    ptr->next = new;
}

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

char	*ft_get_unquoted_str(t_lexer *l)
{
    int   start;
    char  *raw_delim;
    char  *final_delim;
    int   len;

    start = l->pos;
    // Scan for the end of the delimiter word (ends at space or metachar)
    while (l->pos < l->len && !ft_ismeta(l->input[l->pos]) && !ft_isspace(l->input[l->pos]))
        l->pos++;
    // Extract the raw word (e.g., "EOF" or 'EOF')
    raw_delim = ft_substr(l->input, start, l->pos - start);
    if (!raw_delim)
        return (NULL);
    len = ft_strlen(raw_delim);
    // Check for matching quotes and strip them
    if (len >= 2 && (raw_delim[0] == '\'' && raw_delim[len - 1] == '\''))
    {
        final_delim = ft_substr(raw_delim, 1, len - 2);
        free(raw_delim);
        return (final_delim);
    }
    if (len >= 2 && (raw_delim[0] == '\"' && raw_delim[len - 1] == '\"'))
    {
        final_delim = ft_substr(raw_delim, 1, len - 2);
        free(raw_delim);
        return (final_delim);
    }
    // If no quotes, return the raw delimiter
    return (raw_delim);
}
