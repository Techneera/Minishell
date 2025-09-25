/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:38:25 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/09 22:25:54 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*e_ptr;
	int		length;

	length = ft_strlen(s);
	e_ptr = (char *) s + length;
	while (length-- > -1)
	{
		if (*e_ptr == (unsigned char) c)
			return (e_ptr);
		e_ptr--;
	}
	return (NULL);
}
