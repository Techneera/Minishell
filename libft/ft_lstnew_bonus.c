/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:01:43 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/15 11:13:22 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*l_ptr;

	l_ptr = (t_list *) malloc(sizeof(t_list));
	if (!l_ptr)
		return (NULL);
	l_ptr->content = content;
	l_ptr->next = NULL;
	return (l_ptr);
}
