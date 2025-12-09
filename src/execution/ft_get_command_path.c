/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_command_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static void	check_dir(char **arg, t_data *data);
static void	check_exe_and_absolute(char **arg, t_data *data);
static char	*check_path(char **arg, char **env);
static void	ft_free_all(char **str);

char	*get_command_path(char **arg, char **env, t_data *data)
{
	if (!(*arg) || **arg == '\0')
		return (0);
	check_dir(arg, data);
	if (**arg == '/' && access(*arg, X_OK) == 0)
		return (ft_strdup(*arg));
	check_exe_and_absolute(arg, data);
	if (**arg == '.' && (*arg)[1] == '/')
	{
		if (access(*arg, F_OK) != 0)
		{
			message_error(arg[0], ": No such file or directory", 1);
			secure_exit(data, 127);
		}
		if (access(*arg, X_OK) == 0)
			return (ft_strdup(*arg));
		message_error(arg[0], ": Permission denied", 1);
		secure_exit(data, 126);
	}
	return (check_path(arg, env));
}

static char	*check_path(char **arg, char **env)
{
	int			i;
	char		*tmp;
	char		*path;
	char		**paths;

	i = -1;
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

static void	check_dir(char **arg, t_data *data)
{
	struct stat	st;

	if (stat(*arg, &st) == 0 && S_ISDIR(st.st_mode))
	{
		if (**arg == '/' || (*arg)[1] == '/' || \
(*arg)[ft_strlen((*arg)) - 1] == '/')
		{
			message_error(arg[0], ": Is a directory", 1);
			secure_exit(data, 126);
		}
		message_error(arg[0], ": command not found", 1);
		secure_exit(data, 127);
	}
}

static void	check_exe_and_absolute(char **arg, t_data *data)
{
	if (**arg == '/' && access(*arg, F_OK) != 0)
	{
		message_error(arg[0], ": No such file or directory", 1);
		secure_exit(data, 127);
	}
}

static void	ft_free_all(char **str)
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
