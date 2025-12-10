/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static
void	ft_insert_aux(int *j, int index, char **new_args, char **old_args)
{
	while (*j < index)
	{
		new_args[*j] = old_args[*j];
		(*j)++;
	}
}

static
void	ft_cpy_aux(t_list **files, int *j, char **new_args)
{
	while (*files)
	{
		new_args[*j] = ft_strdup((*files)->content);
		(*files) = (*files)->next;
		(*j)++;
	}
}

char	**insert_wildcard_args(char **old_args, t_list *files, int index)
{
	char	**new_args;
	int		old_len;
	int		files_len;
	int		j;
	int		k;

	old_len = 0;
	while (old_args[old_len])
		old_len++;
	files_len = ft_lstsize(files);
	new_args = malloc(sizeof(char *) * (old_len + files_len + 1));
	if (!new_args)
		return (NULL);
	j = 0;
	ft_insert_aux(&j, index, new_args, old_args);
	ft_cpy_aux(&files, &j, new_args);
	k = index + 1;
	while (old_args[k])
		new_args[j++] = old_args[k++];
	new_args[j] = NULL;
	free(old_args[index]);
	free(old_args);
	return (new_args);
}

/* Helper to restore masked chars if no expansion occurred */
void	unmask_wildcards(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\001')
			str[i] = '*';
		i++;
	}
}
