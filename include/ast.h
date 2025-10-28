#ifndef AST_H
# define AST_H

# include "libshell.h"

t_ast		*ft_ast_node_command(t_cmd *cmd);
t_ast		*ft_ast_generic_node(t_node_type type);
t_parser	*ft_init_parser(t_lexer *l);
t_cmd		*ft_create_command(char **av, t_redir *redirs, int count);
t_redir		*ft_create_redir_lst(t_label_redir label, char *str);
void		ft_parser_iter(t_parser *parser);

t_ast		*ft_parser(t_lexer *l);
t_ast		*ft_parse_and_or(t_parser *parser);
t_ast		*ft_parse_pipeline(t_parser *parser);
t_ast		*ft_parse_grain_with_redirs(t_parser *parser);
t_ast		*ft_parse_node_command(t_parser *parser);
t_ast		*ft_parse_subshell(t_parser *parser);
char		**ft_parse_args(t_parser *parser);
char		**ft_lst_to_args(t_list **head, int size);
int			ft_handle_redirects(t_parser *parser, t_redir **head_redir);
void		ft_redirs_addback(t_redir **head, t_redir *new);

void		ft_redirs_clear(t_redir **redirs);
void		ft_free_redir_lst(t_redir *redir);
void		ft_free_array(char **arr);
void		ft_free_cmd(t_cmd *cmd);
void		ft_free_ast(t_ast *root);

#endif
