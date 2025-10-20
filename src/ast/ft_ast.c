#include "libshell.h"

t_ast	*ft_ast_new_cmd(t_cmd *cmd)
{
	t_ast	*new_cmd;

	if (!cmd)
		return (NULL);
	new_cmd= (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_cmd)
		return (NULL);
	new_cmd->type = NODE_CMD;
	new_cmd->cmd = command;
	new_cmd->left = NULL;
	new_cmd->right = NULL;
	return (new_command);
}

t_ast	*ft_ast_new_pipe(t_ast *l, t_ast *r)
{
	t_ast	*new_pipe;

	new_pipe = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_pipe)
		return (NULL);
	new_pipe->type = NODE_PIPE;
	new_pipe->cmd = NULL;
	new_pipe->left = l;
	new_pipe->right = r;
	return (new_pipe);
}

t_cmd	*ft_create_cmd(t_lexer *l)
{
    return (NULL);
}
/*
t_cmd	*ft_generate_command()
*/
