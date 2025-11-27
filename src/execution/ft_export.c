#include "execution.h"

static int	has_arg(char *arg);

void	ft_export(t_list *list, t_ast *node)
{
	int		n_args;
	int 	i;
	t_list 	*new_arg;

	i = 0;
	if (!list || !node)
		return ;
	n_args = ft_arraylen((void **) node->cmd->args);
	if (n_args == 1)
		ft_print_sorted_export(list);
	else
	{
		while (++i < n_args)
		{
			new_arg = create_node_env(node->cmd->args[i], has_arg(node->cmd->args[i]));
			if (!new_arg)
				return ;
			ft_lstadd_back(&list, new_arg);
		}
		ft_print_sorted_export(list);
	}
}

static int	has_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	while (arg[i])
	{
		if (arg[i] == '=')
			return (1);
		i++;
	}
	return (0);
}
