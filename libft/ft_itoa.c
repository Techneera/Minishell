/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:18:16 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/14 11:23:05 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_lenmalloc(int nb)
{
	size_t	i;

	i = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb *= -1;
		i++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

static char	*ft_populate(char *ptr, int n)
{
	char	*this_ptr;

	this_ptr = ptr;
	if (n < 0)
	{
		*this_ptr = '-';
		n *= -1;
		this_ptr++;
	}
	if (n > 9)
		this_ptr = ft_populate(this_ptr, n / 10);
	*this_ptr = n % 10 + '0';
	this_ptr++;
	return (this_ptr);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	char	*ptr_start;
	size_t	len;

	if (n == -2147483648)
	{
		return (ft_strdup("-2147483648"));
	}
	len = ft_lenmalloc(n);
	ptr = (char *) malloc(len + 1);
	if (!ptr)
		return (ptr);
	ptr_start = ptr;
	ft_populate(ptr, n);
	ptr_start[len] = '\0';
	return (ptr_start);
}
/*
#include <stdio.h>
int	main()
{
//	printf("%s\n", ft_itoa(42));
//	printf("%s\n", ft_itoa(-0));
	printf("%s\n", ft_itoa(-10004));
}
*/
