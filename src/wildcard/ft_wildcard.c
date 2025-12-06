/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

int	ft_match_asterisk(char *pattern, char *str)
{
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (ft_match_asterisk(pattern, str))
				return (1);
			str++;
		}
		return (0);
	}
	if (!*pattern && !*str)
		return (1);
	if (*pattern == *str)
		return (ft_match_asterisk(pattern + 1, str + 1));
	return (0);
}

int	ft_match_wildcard(char *pattern, char *name)
{
	int	i;

	i = 0;
	if (name[i] == '.' && pattern[i] != '.')
		return (0);
	return (ft_match_asterisk(pattern, name));
}

t_list	*get_wildcard_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*matches;
	char			*name;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (ft_match_wildcard(pattern, entry->d_name))
		{
			name = ft_strdup(entry->d_name);
			if (name)
				ft_lstadd_back(&matches, ft_lstnew(name));
		}
	}
	closedir(dir);
	return (matches);
}
