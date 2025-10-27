#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast		*ast_root;
	t_parser	*parser;

	parser = ft_init_parser(l);
	if (!parser)
	{
		fprintf(stderr, "Error parser allocate.");
		return (NULL);
	}
	ast_root = ft_parse_and_or(parser); // Search lowest precedence
	if (ast_root != NULL && parser->current_token->tok_label == TOKEN_EOF)
	{
		fprintf(stderr, "Error creating tree.");
		ft_free_ast(ast_root);
		ast_root = NULL;
	}
	free(parser->current_token);
	free(parser->peek);
	free(parser);
	return (ast_root);
}

t_ast	*ft_parse_and_or(t_parser *parser)
{
	t_ast		*new_parent;
	t_ast		*ast_left;
	t_ast		*ast_right;
	t_node_type	op_type;

	ast_left = ft_parse_pipeline(parser);
	if (!ast_left)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_AND || \
			parser->current_token->tok_label == TOKEN_OR)
	{
		if (parser->current_token->tok_label == TOKEN_AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;
		ft_parser_iter(parser);
		ast_right = ft_parse_pipeline(parser);
		if (!ast_right)
			return (ft_free_ast(ast_left), NULL);
		new_parent = ft_ast_generic_node(op_type);
		if (!new_parent)
			return (ft_free_ast(ast_left), ft_free_ast(ast_right), NULL);
		new_parent->left = ast_left;
		new_parent->right= ast_right;
		ast_left = new_parent;
	}
	return (ast_left);
}

t_ast	*ft_parse_pipeline(t_parser *parser)
{
	t_ast	*new_parent;
	t_ast	*ast_left;
	t_ast	*ast_right;

	ast_left = ft_parse_grain_with_redirs(parser);
	if (!ast_left)
		return (NULL);
	while (parser->current_token->tok_label == TOKEN_PIPE)
	{
		ft_parser_iter(parser);
		ast_right = ft_parse_grain_with_redirs(parser);
		if (!ast_right)
			return (NULL);
		new_parent = ft_ast_generic_node(NODE_PIPE);
		if (!new_parent)
			return (NULL);
		new_parent->left = ast_left;
		new_parent->right= ast_right;
		ast_left = new_parent;
	}
	return (ast_left);
}

t_ast	*ft_parse_grain_with_redirs(t_parser *parser)
{
	t_ast	*node;

	node = NULL;
	if (parser->current_token->tok_label == TOKEN_LEFT_PAR)
		node = ft_parse_subshell(parser);
	else if (parser->current_token->tok_label == TOKEN_WORD)
		node = ft_parse_node_command(parser);
	else
		fprintf(stderr, "Error invalid token.");
	if (!node)
		return (NULL);
	ft_handle_redirects(parser, &node);
}

t_ast	*ft_parse_node_command(t_parser *parser)
{
	t_ast	*node_cmd;
	t_cmd	*cmd;

	cmd = ft_create_command(NULL, NULL, 0);
	if (!cmd)
		return (NULL);
	cmd->args = ft_parse_args(parser);
	if (!cmd->args)
		return (free(cmd), NULL);
	node_cmd = ft_ast_node_command(cmd);
	return (node_cmd);
}

char	**ft_parse_args(t_parser *parser)
{
	t_list	*head;
	char	**args_ret;
	char	*arg_cpy;

	head = NULL;
	while (parser->current_token->tok_label == TOKEN_WORD)
	{
		arg_cpy = strdup(parser->current_token->str);
		if (!arg_cpy)
			return (ft_lstclear(&head, &free), NULL);
		ft_lstadd_back(&head, ft_lstnew(arg_cpy));
		ft_parser_iter(parser);
	}
	args_ret = ft_lst_to_args(&head, ft_lstsize(head));
	if (!args_ret)
		return (ft_lstclear(&head, &free), NULL);
	return (args_ret);
}

char	**ft_lst_to_args(t_lst **head, int size)
{
	char	**ret;
	t_lst	*tmp;
	int		i;

	ret = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (*head)
	{
		tmp = *head;
		ret[i] = (char *)tmp->content;
		*head = tmp->next;
		free(tmp);
		i++;
	}
	return (ret);
}
/*
t_ast	*ft_parse_subshell(parser)
{
	
}
*/
