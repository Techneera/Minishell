#include "ast.h"
#include "lexer.h"

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

t_ast	*ft_ast_generic_node(t_node_type type)
{
	t_ast	*new_node;

	new_node = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->cmd = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_parser	*ft_init_parser(t_lexer *l)
{
	t_parser	*parser;

	parser = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->lex = l;
	parser->current_token = get_next_token(l);
	if (!parser->current_token)
	{
		free(parser);
		return (NULL);
	}
	parser->peek = get_next_token(l);
	if (!parser->peek)
	{
		free(parser->current_token);
		free(parser);
		return (NULL);
	}
	return (parser);
}

void	ft_parser_iter(t_parser *parser)
{
	if (parser->current_token->tok_label == TOKEN_EOF)
		return ;
	free_token(parser->current_token);
	parser->current_token = parser->peek;
	parser->peek = get_next_token(parser->lex);
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

t_redir	*ft_create_redir_lst(t_label_redir label, char *str)
{
	t_redir	*r;

	r = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (NULL);
	r->label = label;
	r->file_name = str;
	return (r);
}

void	ft_free_redir_lst(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
		free(redir->file_name);
		free(redir);
		redir = tmp;
	}
}

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	ft_free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	ft_free_array(cmd->args);
	ft_free_redir_lst(cmd->redirs);
	free(cmd);
}

void	ft_free_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->type == NODE_AND || root->type == NODE_PIPE || root->type == NODE_OR)
	{
		ft_free_ast(root->left);
		ft_free_ast(root->right);
	}
	else if (root->type == NODE_SUBSHELL)
		ft_free_ast(root->body);
	ft_free_cmd(root->cmd);
	free(root);
}
