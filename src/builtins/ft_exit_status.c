#include "libshell.h"

static
int	ft_error(int err_no, char *arg, char *err_msg)
{
	int	status;

	if (arg != NULL)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(err_msg, 2);
		ft_putstr_fd("\n", 2);
		perror("");
	}
	else
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(err_msg, 2);
		ft_putstr_fd("\n", 2);
	return (err_no);
}

static
void	ft_exit_print(void)
{
	if (isatty(STDIN_FILENO) || isatty(STDOUT_FILENO))
		ft_putstr_fd("exit", 2);
}

// Unfinished
static
long	ft_strtol(char *str)
{
	int			i;
	long long	res;
	long long	sign;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	res = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		else
			sign = 1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (ft_error(2, str, "numeric argument required."));
		res = res * 10 + str[i] - '0';
		if (res < -214783648 || res >= 2147483647)
		{
			if (res < -214783648)
				return (-214783648);
			else
				return (214783647);
		}
		i++;
	}
	return ((long)(res * sign));
}

// Unfinshed
void	ft_check_num_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (i > 2)
		{
			ft_putendl();
			free_data();
		}
		i++;
	}
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
