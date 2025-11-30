# include "execution.h"

static int	is_home(char *argv);

void	ft_cd(t_ast *node, char **envp)
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

	if (size == 2 && !is_home(node->cmd->args[1]))
	{
		if (node->cmd->args[1][0] == '"' && node->cmd->args[1][1] == '"' && node->cmd->args[1][2] == '\0')
		{
			message_error(": null directory", node->cmd->args[0], 0);
			return ;
		}
		if (chdir(node->cmd->args[1]) != 0)
			no_such_file(node->cmd->args[0], node->cmd->args[1]);	
	}
	else
		chdir(ft_getenv(envp, "HOME"));
}

static int	is_home(char *argv)
{
	if (argv[0] == '~' && argv[1] == '\0')
		return (1);
	return (0);
}
