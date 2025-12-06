#include "libshell.h"
#include "execution.h"

int	ft_exit(t_data *data)
{
	char		**args;
	long long	exit_val;
	int			i;

	ft_exit_print();
	i = 1;
	if (!data->tree)
		secure_exit(data, 0);
	args = data->tree->cmd->args;
	if (!args[1])
		secure_exit(data, ft_exit_status(0, 0, 0));
	if (!ft_strncmp(args[1], "--", 3))
	{
		i = 2;
		if (!args[i])
			secure_exit(data, ft_exit_status(0, 0, 0));
	}
	if (!ft_safe_atoll(args[i], &exit_val))
	{
		ft_exit_status(ft_error(2, args[i], "numeric argument required"), 1, 0);
		secure_exit(data, 2);
	}
	if (args[i + 1])
		return (ft_error(1, NULL, "too many arguments"));
	secure_exit(data, (int)(exit_val % 256));
	return (0);
}

int	ft_exit_status(int state, int write_, int exit_)
{
	static int	current_status;

	if (write_ != 0)
		current_status = state;
	if (exit_ != 0)
		exit(current_status);
	return (current_status);
}
