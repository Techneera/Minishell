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

/**
 * \brief Recursively match a glob pattern (with `*`) against a string.
 *
 * Consecutive `*` characters are collapsed.  A trailing `*` matches
 * any suffix.  Literal characters must match exactly.
 * \param pattern The remaining pattern to match.
 * \param str     The remaining string to test.
 * \return 1 if the pattern matches \c str; 0 otherwise.
 */
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

/**
 * \brief Match a glob pattern against a filesystem entry name.
 *
 * Prevents hidden files (names starting with `.`) from matching unless
 * the pattern itself starts with `.`.  Delegates to ft_match_asterisk().
 * \param pattern The glob pattern.
 * \param name    The filesystem entry name to test.
 * \return 1 if they match; 0 otherwise.
 */
int	ft_match_wildcard(char *pattern, char *name)
{
	int	i;

	i = 0;
	if (name[i] == '.' && pattern[i] != '.')
		return (0);
	return (ft_match_asterisk(pattern, name));
}

static
int	ft_init_globs(t_globs *g, char *pattern)
{
	g->full_match = NULL;
	g->last_slash = ft_strrchr(pattern, '/');
	if (g->last_slash)
	{
		g->dir_prefix = ft_substr(pattern, 0, (g->last_slash - pattern) + 1);
		g->file_pattern = ft_strdup(g->last_slash + 1);
	}
	else
	{
		g->dir_prefix = NULL;
		g->file_pattern = ft_strdup(pattern);
	}
	if ((!g->dir_prefix && g->last_slash) || !g->file_pattern)
	{
		if (g->dir_prefix)
			free(g->dir_prefix);
		return (0);
	}
	return (1);
}

static
void	ft_collect_matches(DIR *dir, t_globs *g, t_list **matches)
{
	struct dirent	*entry;

	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (ft_match_wildcard(g->file_pattern, entry->d_name))
		{
			if (g->dir_prefix)
				g->full_match = ft_strjoin(g->dir_prefix, entry->d_name);
			else
				g->full_match = ft_strdup(entry->d_name);
			if (g->full_match)
				ft_lstadd_back(matches, ft_lstnew(g->full_match));
		}
	}
}

/**
 * \brief Collect all directory entries that match the given glob pattern.
 *
 * Splits the pattern on the last `/` to find the directory and the
 * filename pattern, opens the directory, and calls ft_match_wildcard()
 * on each entry.  Results are returned as a linked list of full paths.
 * \param pattern The glob pattern (may contain a path prefix).
 * \return Linked list of heap-allocated matching path strings, or NULL.
 */
t_list	*get_wildcard_matches(char *pattern)
{
	t_globs	g;
	t_list	*matches;
	DIR		*dir;

	matches = NULL;
	if (!ft_init_globs(&g, pattern))
		return (NULL);
	if (g.dir_prefix)
		dir = opendir(g.dir_prefix);
	else
		dir = opendir(".");
	if (dir)
	{
		ft_collect_matches(dir, &g, &matches);
		closedir(dir);
	}
	if (g.dir_prefix)
		free(g.dir_prefix);
	free(g.file_pattern);
	return (matches);
}
