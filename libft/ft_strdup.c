/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:05:52 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/11 16:45:33 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char			*ptr;
	char			*start_ptr;
	unsigned int	len;

	len = ft_strlen(s) + 1;
	ptr = (char *) malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	start_ptr = ptr;
	while (*s)
	{
		*ptr = *s;
		ptr++;
		s++;
	}
	*ptr = '\0';
	return (start_ptr);
}
