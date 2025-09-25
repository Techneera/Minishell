/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:24:36 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/11 12:22:59 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcat(char *dst, const char *src, unsigned int size)
{
	char			*ptr_d;
	unsigned int	slen;
	unsigned int	dlen;
	unsigned int	offset;

	slen = ft_strlen(src);
	dlen = ft_strlen(dst);
	ptr_d = dst + dlen;
	offset = size - dlen - 1;
	if (size <= dlen)
		return (slen + size);
	while (*src && offset--)
	{
		*ptr_d = *src;
		ptr_d++;
		src++;
	}
	*ptr_d = '\0';
	return (dlen + slen);
}
/*
#include <stdio.h>
#include <bsd/string.h>
int	main()
{
	char	dst[] = "aaaaaaaaaa";
	char	src[] = "LOREM";
	char	s[] = "LOREM";
	char	d[] = "aaaaaaaaaa";

	printf("%zu\n", strlcat(dst, src, 14));
	printf("%d\n", ft_strlcat(d, s, 14));
	printf("%s\n", dst);
	printf("%s\n", d);
}
*/
