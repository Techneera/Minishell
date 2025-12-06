/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "execution.h"

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

void	ft_exit_print(void)
{
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
}

static
void	ft_skip_and_sign(int *i, int *sign, char *str)
{
	while (ft_isspace(str[*i]))
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

int	ft_safe_atoll(char *str, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	res;

	i = 0;
	res = 0;
	sign = 1;
	ft_skip_and_sign(&i, &sign, str);
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
