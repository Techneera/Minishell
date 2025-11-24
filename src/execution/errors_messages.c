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
