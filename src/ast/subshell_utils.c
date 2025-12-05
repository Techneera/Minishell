#include "libshell.h"
#include "ast.h"
#include "lexer.h"

int	ft_proc_redir_loop(t_parser *p, t_list **lst, t_cmd *cmd)
{
	t_redir			*new_r;
	t_list			*link;
	t_label_redir	label;

	label = ft_label_map(p->current_token->tok_label);
	new_r = ft_extract_redir_info(p, label);
	if (!new_r)
		return (0);
	link = ft_lstnew(new_r);
	if (!link)
	{
		ft_free_redir_content(new_r);
		return (0);
	}
	ft_lstadd_back(lst, link);
	cmd->redir_count++;
	ft_parser_iter(p);
	if (label != REDIR_HEREDOCK)
		ft_parser_iter(p);
	return (1);
}

int	ft_isredir(t_parser *parser)
{
	return (parser->current_token->tok_label == TOKEN_REDIR_IN || \
parser->current_token->tok_label == TOKEN_REDIR_OUT || \
parser->current_token->tok_label == TOKEN_REDIR_APPEND || \
parser->current_token->tok_label == TOKEN_REDIR_HEREDOC);
}

void	ft_parse_args(t_parser *parser, t_list **head)
{
	char	*arg_cpy;

	arg_cpy = strdup(parser->current_token->str);
	if (!arg_cpy)
	{
		ft_lstclear(head, &free);
		return ;
	}
	ft_lstadd_back(head, ft_lstnew(arg_cpy));
	ft_parser_iter(parser);
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
		return (fprintf(stderr, "Error unclosed parenthese.\n"), \
ft_free_ast(body), NULL);
	ft_parser_iter(parser);
	node_subsh = ft_ast_generic_node(NODE_SUBSHELL);
	if (!node_subsh)
		return (ft_free_ast(body), NULL);
	node_subsh->body = body;
	return (node_subsh);
}
