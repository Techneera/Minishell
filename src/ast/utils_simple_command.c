#include "libshell.h"
#include "ast.h"
#include "lexer.h"

static
int	ft_handle_tok_word(t_parser *parser, t_container *container)
{
	container->arg_cpy = ft_strdup(parser->current_token->str);
	container->new_link = ft_lstnew(container->arg_cpy);
	if (!container->arg_cpy || !container->new_link)
	{
		free(container->arg_cpy);
		free(container->new_link);
		return (false);
	}
	ft_lstadd_back(&container->args_lst, container->new_link);
	ft_parser_iter(parser);
	return (true);
}

static
int	ft_redirs_constructor(t_parser *parser, t_container *container)
{
	container->new_redir = ft_parse_single_redir(parser);
	if (!container->new_redir)
	{
		ft_lstclear(&container->args_lst, &free);
		ft_lstclear(&container->redirs_lst, &ft_free_redir_content);
		return (false);
	}
	container->new_link = ft_lstnew(container->new_redir);
	if (!container->new_link)
	{
		ft_free_redir_content(container->new_redir);
		ft_lstclear(&container->args_lst, &free);
		ft_lstclear(&container->redirs_lst, &ft_free_redir_content);
		return (false);
	}
	ft_lstadd_back(&container->redirs_lst, container->new_link);
	return (true);
}

static
int	ft_transform(t_cmd **cmd, t_container *container)
{
	*cmd = ft_create_command(NULL, NULL, 0);
	if (!*cmd)
		return (ft_lstclear(&container->args_lst, &free), \
ft_lstclear(&container->redirs_lst, &ft_free_redir_content), \
false);
	(*cmd)->args = ft_lst_to_args(&container->args_lst, \
ft_lstsize(container->args_lst));
	(*cmd)->redir_count = ft_lstsize(container->redirs_lst);
	if ((*cmd)->redir_count > 0)
	{
		(*cmd)->redirs = (t_redir *)ft_calloc((*cmd)->redir_count, \
sizeof(t_redir));
		if (!(*cmd)->redirs)
			return (ft_free_cmd(*cmd), \
ft_lstclear(&container->redirs_lst, &ft_free_redir_content), \
false);
		ft_copy_lst_to_array(container->redirs_lst, (*cmd)->redirs);
		ft_lstclear(&container->redirs_lst, &ft_free_redir_struct_only);
	}
	return (true);
}

static
void	ft_init_lists(t_container *container, t_cmd **cmd)
{
	*cmd = NULL;
	container->args_lst = NULL;
	container->redirs_lst = NULL;
}

t_ast	*ft_parse_simple_command(t_parser *parser)
{
	t_container	container;
	t_cmd		*cmd;
	t_ast		*cmd_node;

	ft_init_lists(&container, &cmd);
	while (parser->current_token->tok_label == TOKEN_WORD || ft_isredir(parser))
	{
		if (parser->current_token->tok_label == TOKEN_WORD)
		{
			if (!ft_handle_tok_word(parser, &container))
				return (NULL);
		}
		else if (ft_isredir(parser))
		{
			if (ft_redirs_constructor(parser, &container))
				return (NULL);
		}
	}
	if (!ft_transform(&cmd, &container))
		return (NULL);
	cmd_node = ft_ast_node_command(cmd);
	if (!cmd_node)
		return (ft_free_cmd(cmd), NULL);
	return (cmd_node);
}
