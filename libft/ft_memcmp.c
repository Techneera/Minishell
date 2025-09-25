/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:56:53 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/08 09:07:14 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, unsigned int n)
{
	unsigned char	*ptr_s1;
	unsigned char	*ptr_s2;

	ptr_s1 = (unsigned char *) s1;
	ptr_s2 = (unsigned char *) s2;
	while (n--)
	{
		if (*ptr_s1 != *ptr_s2)
			return (*ptr_s1 - *ptr_s2);
		ptr_s1++;
		ptr_s2++;
	}
	return (0);
}
