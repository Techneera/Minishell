#include "libshell.h"
#include "execution.h"

static
int	ft_error(int err_no, char *arg, char *err_msg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg)
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
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
}

static int	ft_safe_atoll(char *str, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	res;

	i = 0;
	res = 0;
	sign = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if (res > (unsigned long long)LLONG_MAX / 10 || \
(res == (unsigned long long)LLONG_MAX / 10 && \
(str[i] - '0') > (LLONG_MAX % 10 + (sign == -1))))
			return (0);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	*out = (long long)(res * sign);
	return (1);
}

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
