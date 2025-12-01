#include "libshell.h"
#include "execution.h"

static
int	ft_error(int err_no, char *arg, char *err_msg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg != NULL)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
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

static
long	ft_strtol(char *str)
{
	int			i;
	long long	res;
	long long	sign;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	sign = 1;
	res = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (ft_error(2, str, "numeric argument required."));
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

static
int	ft_check_num_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i - 1);
}

int	ft_exit(t_data *data)
{
	char	**args;
	int		n_args;
	long	exit_code_l;

	ft_exit_print();
	args = data->tree->cmd->args;
	n_args = ft_check_num_args(args);
	if (n_args > 1)
		return (ft_error(1, NULL, "too many arguments"));
	if (n_args == 0)
		secure_exit(data, data->status);
	else
	{
		exit_code_l = ft_strtol(args[1]);
		if (exit_code_l == 2)
			secure_exit(data, 2); 
		else
			secure_exit(data, (int)exit_code_l);
	}
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
