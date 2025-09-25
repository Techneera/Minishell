/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:37:41 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/08/12 11:39:33 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(int c)
{
	unsigned char	ch;

	ch = (unsigned char) c;
	if ((ch >= 9 && ch <= 13) || (ch == 32))
		return (TRUE);
	else
		return (FALSE);
}
