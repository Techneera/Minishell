/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:26:59 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/11 10:33:35 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, unsigned int n)
{
	unsigned char	*ptr_dest;
	unsigned char	*ptr_src;

	if (dest == NULL && src == NULL)
		return (NULL);
	ptr_src = (unsigned char *) src;
	ptr_dest = (unsigned char *) dest;
	while (n--)
	{
		*ptr_dest = *ptr_src;
		ptr_dest++;
		ptr_src++;
	}
	return (dest);
}
