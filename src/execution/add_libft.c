/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

void	secure_close(int *fd)
{
	if (!fd)
		return ;
	if (*fd == -1)
		return ;
	close(*fd);
	(*fd) = -1;
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	free_all(void **ptr, size_t rows)
{
	size_t	i;

	i = 0;
	if (!ptr)
		return ;
	while (i < rows)
	{
		if (ptr[i])
		{
			free(ptr[i]);
			ptr[i] = NULL;
		}
		i++;
	}
	free(ptr);
	ptr = NULL;
}

int	ft_arraylen(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (i);
	while (ptr[i])
		i++;
	return (i);
}

char	**ft_realloc_empty(char **args)
{
	char	**res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (*args[i] == '\0')
			j++;
		i++;
	}
	res = ft_calloc(i - j + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	j = -1;
	while (args[i])
	{
		if (*args[i] != '\0')
			res[++j] = ft_strdup(args[i]);
		i++;
	}
	ft_free_array(args);
	return (res);
}
