#include "ast.h"

t_ast	*ft_ast_node_command(t_cmd *cmd)
{
	t_ast	*new_node;

	if (!cmd)
		return (NULL);
	new_node = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = NODE_CMD;
	new_node->cmd = cmd;
	new_node->body = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_ast	*ft_ast_generic_node(t_token *token)
{
	t_ast	*new_node;

	new_node = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = ft_which_token();
	new_node->cmd = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_ast	*ft_init_parser(t_lexer *l)
{
	t_ast	*new;

	new = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new)
		return (NULL);
	new->lex = l;
	new->next_token = get_next_token(l);
}

t_ast	*ft_parser_input(t_lexer *l)
{
	t_ast	*node;

	node = ft_init_node(l);
}

t_cmd	*ft_create_command(char	**av, t_redir *redirs, int count)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = av;
	new_cmd->redir_count = count;
	new_cmd->redirs = redirs;
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

t_parser	*ft_init_parser(t_lexer *lex, t_token *)
{
	t_parser	*new;

	new = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!new)
		return (NULL);
	new->lex = NULL;
	new->next_token = NULL;
	new->peek = NULL;
	return (new);
}

void	ft_free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	free(cmd->redirs);
	free(cmd);
}

void	ft_free_node(t_ast *root)
{
	free(root->left);
	free(root->right);
	ft_free_cmd(root->cmd);
	free(root);
}
