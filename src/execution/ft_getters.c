#include "execution.h"

void	message_error(char	*str, char *file, int type);


void	ft_free_all(char **str)
{
	int	i;

	i = -1;
	if (!str)
		return ;
	while (str && str[++i])
	{
		free(str[i]);
		str[i] = NULL;
	}
	free(str);
	str = NULL;
}

void	message_error(char	*str, char *file, int type)
{
	ft_putstr_fd("minishell", 2);
	if (type)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(file, 2);
	}
	else
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("\n\0", 2);
}

char	*get_shell(char **env)
{
	char	**str;
	char	*shell;
	int		i;

	i = -1;
	while (env && env[++i])
	{
		if (ft_strncmp(env[i], "SHELL=", 6) == 0)
			break ;
	}
	if (env && env[i])
	{
		str = ft_split(*(env + i), '/');
		i = -1;
		while (str && str[++i])
			;
		shell = ft_strdup(str[i - 1]);
		ft_free_all(str);
		return (shell);
	}
	return (NULL);
}

char	**get_paths(char **env)
{
	char	**paths;
	char	**paths2;
	int		i;

	i = -1;
	while (env && env[++i])
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
	if (env && env[i])
	{
		paths = ft_split(env[i], '=');
		paths2 = ft_split(paths[1], ':');
		ft_free_all(paths);
		return (paths2);
	}
	return (NULL);
}

char	*get_command_path(char **arg, char **env)
{
	int		i;
	char	*tmp;
	char	*path;
	char	**paths;

	i = -1;
	if (!(*arg) || **arg == '\0')
		return (0);
	if (**arg == '/' && access(*arg, X_OK) == 0)
		return (ft_strdup(*arg));
	paths = get_paths(env);
	while (paths && paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, *arg);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (ft_free_all(paths), path);
		free(path);
	}
	return (ft_free_all(paths), NULL);
}
