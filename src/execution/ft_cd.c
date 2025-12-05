#include "execution.h"

static int		is_home(char *argv);
static void		change_pwd(t_data *data, char *path);
static int		create_path(char *env, char *env_value, t_data *data);

int	ft_cd(t_ast *node, char **envp, t_data *data)
{
	int		size;
	char	buff[PATH_MAX];

	if (!node)
		return (FAIL_STATUS);
	size = ft_arraylen((void **) node->cmd->args);
	if (size > 2)
		return (message_error(": too many arguments", node->cmd->args[0], 0), \
		2);
	if (size == 2 && !is_home(node->cmd->args[1]))
	{
		if (node->cmd->args[1][0] == '"' && \
node->cmd->args[1][1] == '"' && node->cmd->args[1][2] == '\0')
			return (message_error(": null directory", node->cmd->args[0], 0), \
			FAIL_STATUS);
		if (chdir(node->cmd->args[1]) != 0)
			return (no_such_file(node->cmd->args[0], node->cmd->args[1]), 1);
	}
	else
		chdir(ft_getenv(envp, "HOME"));
	if (!getcwd(buff, sizeof(buff)))
		ft_putstr_fd("cd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 2);
	change_pwd(data, buff);
	return (0);
}

static int	is_home(char *argv)
{
	if (argv[0] == '~' && argv[1] == '\0')
		return (1);
	return (0);
}

static void	change_pwd(t_data *data, char *path)
{
	char	*pwd;

	create_path("OLDPWD=", ft_getenv(data->envp, "PWD"), data);
	if (path && *path)
	{
		if (!create_path("PWD=", path, data))
			return ;
	}
	else
	{
		pwd = ft_strjoin(ft_getenv(data->envp, "PWD"), "/..");
		if (!pwd)
			return ;
		create_path("PWD=", pwd, data);
		free (pwd);
	}
}

static
int	create_path(char *env, char *env_value, t_data *data)
{
	char	*args[3];
	char	*str;

	args[0] = "export";
	args[2] = NULL;
	str = ft_strjoin(env, env_value);
	if (!str)
		return (ft_exit_status(FAIL_STATUS, 1, 0), 0);
	args[1] = str;
	if (ft_export(data->env_list, args, data) == FAIL_STATUS)
		return (free(str), ft_exit_status(FAIL_STATUS, 1, 0), 0);
	return (free(str), 1);
}
