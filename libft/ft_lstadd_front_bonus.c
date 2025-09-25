/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:47:24 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/15 12:40:44 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
#include <stdio.h>
int	main()
{
	t_list	**head;
	int	x = 42;
	int y = 4242;
	t_list	*l1;
	t_list	*l2;

	head = (t_list**) malloc(sizeof(t_list *));
	l1 = ft_lstnew((void *) &x);
	*head = l1;
	l2 = ft_lstnew((void *) &y);
	ft_lstadd_front(head, l2);
	printf("%d, %d\n", *(int*) (*head)->content, *(int*) (*head)->next->content);
}
*/
