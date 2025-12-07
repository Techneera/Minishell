/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_exp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:14:50 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/07 16:15:33 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libshell.h"
#include "lexer.h"

char	*ft_heredoc_quotes(char **raw_delim, char **final_delim, int len)
{
	if (len >= 2 && ((*raw_delim)[0] == '\'' && (*raw_delim)[len - 1] == '\''))
	{
		*final_delim = ft_substr(*raw_delim, 1, len - 2);
		free(*raw_delim);
		return (*final_delim);
	}
	else if (len >= 2 && ((*raw_delim)[0] == '\"' && \
(*raw_delim)[len - 1] == '\"'))
	{
		*final_delim = ft_substr(*raw_delim, 1, len - 2);
		free(*raw_delim);
		return (*final_delim);
	}
	return (*raw_delim);
}

void	initialize_and_find_heredoc_delim(int *start, \
int *in_quote_state, t_lexer *l)
{
	*start = l->pos;
	*in_quote_state = 0;
	while (l->pos < l->len)
	{
		if (*in_quote_state)
		{
			if (l->input[l->pos] == *in_quote_state)
				*in_quote_state = 0;
			l->pos++;
		}
		else
		{
			if (l->input[l->pos] == '\'' || l->input[l->pos] == '\"')
				*in_quote_state = l->input[l->pos++];
			else if (ft_ismeta(l->input[l->pos]) || \
ft_isspace(l->input[l->pos]))
				break ;
			else
				l->pos++;
		}
	}
}
