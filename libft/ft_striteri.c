/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:47:41 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/12 18:24:13 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	i = 0;
	while (*s)
	{
		f(i, s);
		i++;
		s++;
	}
}
/*
void    ft(unsigned int n, char *s)
{
    if (s[n] >= 'a' && s[n] <= 'z')
        s[n] = ft_toupper(s[n]);
    else
        s[n] = ft_tolower(s[n]);
}

#include <stdio.h>
int main()
{
    char    s[] = "LoReM iPsUm";
    ft_striteri(s, &ft);
    printf("%s",s);
}
*/
