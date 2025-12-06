#include "libshell.h"
#include "execution.h"

int	ft_unset(t_data *data)
{
	int		i;
	int		ret;
	char	**args;

	args = data->tree->cmd->args;
	i = 1;
	ret = 0;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!ft_is_valid_arguments(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			remove_env_var(data, args[i]);
		i++;
	}
	if (ret == 0 || i > 1)
		refresh_env_array(data);
	return (ret);
}
