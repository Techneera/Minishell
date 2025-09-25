/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:35:55 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/11 12:03:22 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	ch;

	ptr = (unsigned char *) s;
	ch = (unsigned char) c;
	i = ft_strlen(s);
	if (ch == '\0')
		return ((char *)s + i);
	if (*ptr == '\0')
		return (NULL);
	while (i--)
	{
		if (*ptr == ch)
			return ((char *)ptr);
		ptr++;
	}
	return (0);
}
