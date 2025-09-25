/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:24:05 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/08 09:25:52 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, unsigned int n)
{
	unsigned char	*ptr_s;
	unsigned char	ch;

	ptr_s = (unsigned char *) s;
	ch = (unsigned char) c;
	while (n--)
	{
		if (*ptr_s == ch)
			return (ptr_s);
		ptr_s++;
	}
	return (NULL);
}
