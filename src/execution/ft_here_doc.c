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

static char	*my_strjoin(char **s1, char *s2);
static int	reciving_string(char **str, char **line, char *new_lim);

int	make_lim(char **new_lim, char *lim)
{
	*new_lim = NULL;
	*new_lim = ft_strjoin(lim, "\n");
	if (!(*new_lim))
	{
		perror("make_lim malloc failed");
		return (0);
	}
	return (1);
}

int	here_doc(char *lim, int **fd)
{
	char	*str;
	char	*line;

	str = NULL;
	line = NULL;
	if (!reciving_string(&str, &line, lim))
		return (0);
	if (line)
	{
		ft_putstr_fd(line, (*fd)[1]);
		ft_putstr_fd("\n", (*fd)[1]);
	}
	if (str)
		free(str);
	if (line)
		free(line);
	close((*fd)[1]);
	(*fd)[1] = -1;
	if (ft_exit_status(0, 0, 0) == 130)
	{
		close((*fd)[1]);
		(*fd)[0] = -1;
		return (0);
	}
	return (1);
}

static int	reciving_string(char **str, char **line, char *new_lim)
{
	int					stdin_cpy;

	signal(SIGINT, &handle_sigstop_heredoc);
	stdin_cpy = dup(STDIN_FILENO);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		(*str) = readline("> ");
		if (!((*str) && ft_strncmp((*str), new_lim, ft_max(
						ft_strlen((*str)), ft_strlen(new_lim))) != 0))
			break ;
		(*line) = my_strjoin(line, (*str));
		(*line) = my_strjoin(line, "\n");
		if (!*line)
			return (free((*str)), 0);
		free((*str));
	}
	dup2(stdin_cpy, STDIN_FILENO);
	if (stdin_cpy)
		close(stdin_cpy);
	return (1);
}

static char	*my_strjoin(char **s1, char *s2)
{
	int		i;
	char	*new_s;

	new_s = NULL;
	i = 0;
	new_s = (char *) malloc((ft_strlen(*s1)
				+ ft_strlen(s2) + 1) * sizeof(char));
	if (!new_s)
		return (0);
	while (*s1 && (*s1)[i])
	{
		new_s[i] = (*s1)[i];
		i++;
	}
	while (s2 && *s2)
	{
		new_s[i++] = *s2;
		s2++;
	}
	if (*s1)
		free(*s1);
	new_s[i] = '\0';
	return (new_s);
}
