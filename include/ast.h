#ifndef AST_H
# define AST_H

# include "libshell.h"

typedef struct s_parser
{
	t_lexer	*lex;
	t_token	*next_token;
	t_token	*peek;
}	t_parser;

t_ast	*ft_ast_node_command(t_cmd *cmd);
t_ast	*ft_ast_generic_node(t_node_type type);
t_ast	*ft_init_parser(t_lexer *l);
t_ast	*ft_parse_input(t_lexer *l);
t_cmd	*ft_create_command(char **av, t_redir *redirs, int count);
t_redir	*ft_create_redir(t_label_redir label, char *str);
void	ft_free_cmd(t_cmd *cmd);
void	ft_free_node(t_ast *root);

#endif
