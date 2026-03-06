/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "ast.h"
#include "lexer.h"

/**
 * \brief Allocate a t_redir with the given label and filename string.
 * \param label The redirection type.
 * \param str   Heap-allocated filename (ownership transferred; freed on failure).
 * \return Heap-allocated \c t_redir, or NULL on allocation failure.
 */
t_redir	*ft_create_redir(t_label_redir label, char *str)
{
	t_redir	*r;

	r = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (free(str), NULL);
	r->label = label;
	r->file_name = str;
	return (r);
}

/**
 * \brief Free a t_redir and its filename string.
 *
 * Used as a linked-list destructor for redir lists where the string
 * has NOT yet been transferred to a flat array.
 * \param content The \c t_redir pointer cast to void*.
 */
void	ft_free_redir_content(void *content)
{
	t_redir	*redir;

	redir = (t_redir *)content;
	if (!redir)
		return ;
	free(redir->file_name);
	free(redir);
}

/**
 * \brief Copy redirection nodes from a linked list into a pre-allocated array.
 *
 * Transfers the \c label and \c file_name from each list node into the
 * corresponding array slot (shallow copy — strings are not duplicated).
 * \param head  The source linked list.
 * \param array The destination array (must have enough slots pre-allocated).
 */
void	ft_copy_lst_to_array(t_list *head, t_redir *array)
{
	int		i;
	t_list	*tmp;
	t_redir	*redir_to_arr;

	i = 0;
	tmp = head;
	while (tmp)
	{
		redir_to_arr = (t_redir *)tmp->content;
		array[i].label = redir_to_arr->label;
		array[i].file_name = redir_to_arr->file_name;
		i++;
		tmp = tmp->next;
	}
}
