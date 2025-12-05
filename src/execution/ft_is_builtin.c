# include "execution.h"

int	ft_is_builtin(t_data *data, char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "echo\0", 5) == 0)
		return (ft_exit_status(ft_echo(data), 1, 0), 1);
	if (ft_strncmp(arg, "cd\0", 3) == 0)
		return (ft_exit_status(ft_cd(data->tree, data->envp, data), 1, 0), 1);
	if (ft_strncmp(arg, "pwd\0", 4) == 0)
		return (ft_exit_status(ft_pwd(data), 1, 0), 1);
	if (ft_strncmp(arg, "export\0", 7) == 0)
		return (ft_exit_status(ft_export(
					data->env_list, data->tree->cmd->args, data), 1, 0), 1);
	if (ft_strncmp(arg, "unset\0", 6) == 0)
		return (ft_exit_status(ft_unset(data), 1, 0), 1);
	if (ft_strncmp(arg, "env\0", 4) == 0)
		return (ft_exit_status(ft_env(data), 1, 0), 1);
	if (ft_strncmp(arg, "exit\0", 5) == 0)
		return (ft_exit_status(ft_exit(data), 1, 0), 1);
	return (0);
}
