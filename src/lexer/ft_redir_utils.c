/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "lexer.h"

t_token	*ft_which_redir(t_lexer *l)
{
	if (l->input[l->pos] == '>')
		return (ft_handle_output_redir_aux(l));
	if (l->input[l->pos] == '<')
	{
		if (l->pos + 1 < l->len && l->input[l->pos + 1] == '<')
			return (ft_handle_heredoc_aux(l));
		l->pos++;
		return (create_token(TOKEN_REDIR_IN, ft_strdup("<")));
	}
	return (create_token(TOKEN_ERROR, \
ft_strdup("minishell: syntax error near unexpected token `<'")));
}

t_token	*ft_handle_output_redir_aux(t_lexer *l)
{
	if (l->pos + 1 < l->len && l->input[l->pos + 1] == '>')
	{
		l->pos += 2;
		return (create_token(TOKEN_REDIR_APPEND, ft_strdup(">>")));
	}
	l->pos++;
	return (create_token(TOKEN_REDIR_OUT, ft_strdup(">")));
}

t_token	*ft_handle_heredoc_aux(t_lexer *l)
{
	char	*delim;

	l->pos += 2;
	while (l->pos < l->len && ft_isspace(l->input[l->pos]))
		l->pos++;
	delim = ft_get_unquoted_str(l);
	if (!delim)
		return (create_token(TOKEN_ERROR, \
ft_strdup("Unexpected error in delim.")));
	return (create_token(TOKEN_REDIR_HEREDOC, delim));
}
