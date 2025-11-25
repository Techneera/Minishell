#include "libshell.h"
#include "execution.h"

static
int	is_valid_n_option(char *arg)
{
	int		i;

	i = 1;
	if (!arg || arg[0] != '-' || arg[1] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_data *data)
{
	char	**args;
	int		i;
	int		print_newline;

	args = data->tree->cmd->args;
	print_newline = 1;
	i = 1;
	while (args[i])
	{
		if (is_valid_n_option(args[i]) == 1)
		{
			print_newline = 0;
			i++;
		}
		else
			break ;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1] != NULL)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (print_newline == 1)
		ft_putstr_fd("\n", 1);
	return (0);
}
