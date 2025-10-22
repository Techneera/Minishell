#include "libshell.h"

t_ast	*ft_ast_node(t_cmd *cmd)
{
	t_ast	*new_node;

	if (!cmd)
		return (NULL);
	new_node= (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = NODE_EMPTY;
	new_node->cmd = cmd;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_cmd	*ft_create_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->redir_count = 0;
	new_cmd->redirs = NULL;
    return (new_cmd);
}

t_redir	*ft_create_redir(t_label_redir label, char *str)
{
	t_redir	*r;

	r = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (NULL);
	r->label = label;
	r->file_name = str;
	return (r);
}
