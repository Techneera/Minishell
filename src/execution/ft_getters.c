#include "execution.h"

void	message_error(char	*str, char *file, char **env, int type);

void	get_fd_file(int fd_file[2], char *read_file,
			char *write_file, char **env)
{
	if (access(read_file, F_OK) != 0)
	{
		fd_file[0] = -1;
		message_error(": no such file or directory: ", read_file, env, 0);
	}
	else
	{
		fd_file[0] = open(read_file, O_RDONLY);
		if (fd_file[0] == -1)
			message_error(": Permission denied", read_file, env, 0);
	}
	fd_file[1] = open(write_file, O_WRONLY | O_CREAT | O_TRUNC, 0671);
	if (fd_file[1] == -1)
		message_error(": Permission denied", write_file, env, 0);
}

void	message_error(char	*str, char *file, char **env, int type)
{
	char	*shell;

	shell = get_shell(env);
	ft_putstr_fd(shell, 2);
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
	if (shell)
		free(shell);
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
