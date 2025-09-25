/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:43:00 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/10 10:58:36 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	len;

	len = ft_strlen(src);
	if (size > 0)
	{
		while (--size && *src)
		{
			*dst = *src;
			dst++;
			src++;
		}
		*dst = '\0';
	}
	return (len);
}
