#include "libshell.c"

t_ast	*ft_ast_new_cmd(t_cmd *command)
{
	t_ast	*new_command;

	if (!command)
		return (NULL);
	new_command = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_command)
		return (NULL);
	new_command->type = NODE_CMD;
	new_command->cmd = command;
	new_command->left = NULL;
	new_command->right = NULL;
	return (new_command);
}

t_ast	*ft_ast_new_pipe(t_ast *left_subtree, t_ast *right_subtree)
{
	t_ast	*new_pipe;

	new_pipe = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_pipe)
		return (NULL);
	new_pipe->type = NODE_PIPE;
	new_pipe->cmd = NULL;
	new_pipe->left = left_subtree;
	new_pipe->right = right_subtree;
	return (new_pipe);
}

//t_cmd	*ft_generate_command()
