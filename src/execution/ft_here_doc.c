#include "execution.h"

static char	*my_strjoin(char **s1, char *s2);


void	make_lim(char **new_lim, char *lim)
{
	*new_lim = NULL;
	*new_lim = ft_strjoin(lim, "\n");
	if (!(*new_lim))
		exit(1);
}

int	here_doc(char *lim)
{
	char	*str;
	char	*line;
	char	*new_lim;
	int		fd[2];

	if (pipe(fd) == -1)
		exit(1);
	str = NULL;
	line = NULL;
	make_lim(&new_lim, lim);
	ft_printf("> ");
	while (1)
	{
		str = get_next_line (0);
		if (!(str && ft_strncmp(str, new_lim, ft_max(
				ft_strlen(str), ft_strlen(new_lim))) != 0))
			break ;
		line = my_strjoin(&line, str);
		if (!line)
			exit(1);
		free(str);
		ft_printf("> ");
	}
	ft_putstr_fd(line, fd[1]);
	if (str)
		free(str);
	if (line)
		free(line);
	if (new_lim)
		free(new_lim);
	close(fd[1]);
	fd[1] = -1;
	return (fd[0]);
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
