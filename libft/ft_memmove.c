/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:15:40 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/08 09:23:46 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, unsigned int n)
{
	unsigned char	*ptr_src;
	unsigned char	*ptr_dest;

	if (n == 0)
		return (dest);
	if (dest == NULL && src == NULL)
		return (NULL);
	ptr_src = (unsigned char *) src;
	ptr_dest = (unsigned char *) dest;
	if (ptr_dest < ptr_src)
		ft_memcpy(dest, src, n);
	else
	{
		while (n-- && *ptr_src)
		{
			*(ptr_dest + n) = *(ptr_src + n);
		}
	}
	return (dest);
}
/*
int main()
{
	char	*str;
	str = "ThisIsAString";
	int len = ft_strlen(str);

}
*/
