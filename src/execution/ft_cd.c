# include "execution.h"

void	ft_cd(t_ast *node)
{
	int	size;

	if (!node)
		return ;
	size = ft_arraylen((void **) node->cmd->args);
	if (size > 2)
	{
		message_error(": too many arguments", node->cmd->args[0], 0);
		return ;
	}
	if (size == 2)
		chdir(node->cmd->args[1]);
	else
		chdir(getenv("HOME"));
}
