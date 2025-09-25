/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:18:56 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/09 14:20:44 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, unsigned int len)
{
	unsigned int	n;

	if (*big == '\0' && *little == '\0' && len == 0)
		return ((char *) big);
	if (big == little)
		return ((char *) big);
	if (*little == '\0' && len == 0)
		return ((char *) big);
	n = ft_strlen(little);
	while (len && *big)
	{
		if (!ft_strncmp(big, little, n) && n <= len)
			return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}
/*
int	main()
{
	
}*/
