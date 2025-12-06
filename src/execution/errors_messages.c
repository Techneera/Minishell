#include "execution.h"

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

void	no_such_file(char *cmd, char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(" : ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

void	export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}
