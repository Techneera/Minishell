#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_ast	*ft_parser(t_lexer *l)
{
	t_ast		*ast_root;
	t_parser	*parser;

	parser = ft_init_parser(l);
	if (!parser)
	{
		fprintf(stderr, "Error parser allocate.\n");
		return (NULL);
	}
	ast_root = ft_parse_and_or(parser); // Search lowest precedence
	if (ast_root != NULL && parser->current_token->tok_label != TOKEN_EOF)
	{
		fprintf(stderr, "Syntax error near unexpected token: %s.\n", parser->current_token->str);
		ft_free_ast(ast_root);
		ast_root = NULL;
	}
	free_token(parser->current_token);
	free_token(parser->peek);
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
			return (ft_free_ast(ast_left), NULL);
		new_parent = ft_ast_generic_node(NODE_PIPE);
		if (!new_parent)
			return (ft_free_ast(ast_left), ft_free_ast(ast_right), NULL);
		new_parent->left = ast_left;
		new_parent->right= ast_right;
		ast_left = new_parent;
	}
	return (ast_left);
}

t_ast	*ft_parse_grain_with_redirs(t_parser *parser)
{
	t_ast	*node;
	t_cmd	*cmd_to_fill;

	node = NULL;
	if (parser->current_token->tok_label == TOKEN_LEFT_PAR)
		node = ft_parse_subshell(parser);
	else if (parser->current_token->tok_label == TOKEN_WORD)
		node = ft_parse_node_command(parser);
	else
		fprintf(stderr, "Error invalid token.\n");
	if (!node)
		return (NULL);
	if (node->type == NODE_SUBSHELL)
	{
		node->cmd = ft_create_command(NULL, NULL, 0);
		if (!node->cmd)
			return (ft_free_ast(node), NULL);
	}
	cmd_to_fill = node->cmd;
	if (ft_handle_redirects(parser, cmd_to_fill) == false)
		return (ft_free_ast(node), NULL);
	return (node);
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
	if (!node_cmd)
		return (ft_free_cmd(cmd), NULL);
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

char	**ft_lst_to_args(t_list **head, int size)
{
	char	**ret;
	t_list	*tmp;
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

static
int	ft_isredir(t_parser *parser)
{
	return (parser->current_token->tok_label == TOKEN_REDIR_IN || \
			parser->current_token->tok_label == TOKEN_REDIR_OUT || \
			parser->current_token->tok_label == TOKEN_REDIR_APPEND || \
			parser->current_token->tok_label == TOKEN_REDIR_HEREDOC);
}

static
t_label_redir	ft_label_map(t_token_label label)
{
	if (label == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (label == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (label == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (label == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOCK);
	return (REDIR_NONE);
}

int	ft_handle_redirects(t_parser *parser, t_cmd *cmd_to_fill)
{
	t_redir			*new_redir;
	t_list			*tmp_lst_head;
	t_list			*new_link;
	t_label_redir	label;
	char			*filename;
	
	tmp_lst_head = NULL;
	while (ft_isredir(parser))
	{
		label = ft_label_map(parser->current_token->tok_label);
		if (label == REDIR_HEREDOCK)
			filename = ft_strdup(parser->current_token->str);
		else
		{
			if (parser->peek->tok_label != TOKEN_WORD)
				return (ft_lstclear(&tmp_lst_head, &ft_free_redir_content), false);
			filename = ft_strdup(parser->peek->str);
		}
		if (!filename)
			return (ft_lstclear(&tmp_lst_head, &ft_free_redir_content), false);
		new_redir = ft_create_redir(label, filename);
		if (!new_redir)
			return (ft_lstclear(&tmp_lst_head, &ft_free_redir_content), false);
		new_link = ft_lstnew(new_redir);
		if (!new_link)
			return (ft_free_redir_content(new_redir), \
					ft_lstclear(&tmp_lst_head, &ft_free_redir_content), false);
		ft_lstadd_back(&tmp_lst_head, new_link);
		cmd_to_fill->redir_count++;
		ft_parser_iter(parser);
		if (label != REDIR_HEREDOCK)
			ft_parser_iter(parser);
	}
	if (cmd_to_fill->redir_count > 0)
	{
		cmd_to_fill->redirs = (t_redir *)ft_calloc(cmd_to_fill->redir_count, \
				sizeof(t_redir));
		if (!cmd_to_fill->redirs)
			return (ft_lstclear(&tmp_lst_head, &ft_free_redir_content), false);
		ft_copy_lst_to_array(tmp_lst_head, cmd_to_fill->redirs);
		ft_lstclear(&tmp_lst_head, &ft_free_redir_struct_only);
	}
	return (true);
}

t_ast	*ft_parse_subshell(t_parser *parser)
{
	t_ast	*node_subsh;
	t_ast	*body;

	if (parser->peek->tok_label == TOKEN_RIGHT_PAR)
	{
		ft_parser_iter(parser);
		ft_parser_iter(parser);
		return (fprintf(stderr, "Syntax error near token \")\".\n"), NULL);
	}
	ft_parser_iter(parser);
	body = ft_parse_and_or(parser);
	if (!body)
		return (fprintf(stderr, "Error in subshell body.\n"), NULL);
	if (parser->current_token->tok_label != TOKEN_RIGHT_PAR)
		return (fprintf(stderr, "Error unclosed parenthese.\n"), ft_free_ast(body), NULL);
	ft_parser_iter(parser);
	node_subsh = ft_ast_generic_node(NODE_SUBSHELL);
	if (!node_subsh)
		return (ft_free_ast(body), NULL);
	node_subsh->body = body;
	return (node_subsh);
}
