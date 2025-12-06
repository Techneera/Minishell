/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "lexer.h"

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

t_token	*ft_handle_word(t_lexer *l)
{
	int		start;
	char	in_quotes;

	start = l->pos;
	in_quotes = '\0';
	while (l->pos < l->len)
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
				break ;
		}
		l->pos++;
	}
	if (in_quotes)
		return (create_token(TOKEN_ERROR, ft_strdup("Unclosed word.")));
	else
		return (create_token(TOKEN_WORD, \
ft_substr(l->input, start, l->pos - start)));
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

char	*ft_get_unquoted_str(t_lexer *l)
{
	int		start;
	char	*raw_delim;
	char	*final_delim;
	int		len;

	start = l->pos;
	while (l->pos < l->len && !ft_ismeta(l->input[l->pos]) && \
!ft_isspace(l->input[l->pos]))
		l->pos++;
	raw_delim = ft_substr(l->input, start, l->pos - start);
	if (!raw_delim)
		return (NULL);
	len = ft_strlen(raw_delim);
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
	return (raw_delim);
}
